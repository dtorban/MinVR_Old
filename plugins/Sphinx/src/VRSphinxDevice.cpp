/* -*- c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ====================================================================
 * Copyright (c) 1999-2010 Carnegie Mellon University.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced
 * Research Projects Agency and the National Science Foundation of the
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*
 * continuous.c - Simple pocketsphinx command-line application to test
 *                both continuous listening/silence filtering from microphone
 *                and continuous file transcription.
 */

/*
 * This is a simple example of pocketsphinx application that uses continuous listening
 * with silence filtering to automatically segment a continuous stream of audio input
 * into utterances that are then decoded.
 *
 * Remarks:
 *   - Each utterance is ended when a silence segment of at least 1 sec is recognized.
 *   - Single-threaded implementation for portability.
 *   - Uses audio library; can be replaced with an equivalent custom library.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#else
#include <sys/select.h>
#endif

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>

#include "pocketsphinx.h"

static const arg_t cont_args_def[] = {
    POCKETSPHINX_OPTIONS,
    /* Argument file. */
    {"-argfile",
     ARG_STRING,
     NULL,
     "Argument file giving extra arguments."},
    {"-adcdev",
     ARG_STRING,
     NULL,
     "Name of audio device to use for input."},
    {"-infile",
     ARG_STRING,
     NULL,
     "Audio file to transcribe."},
    {"-inmic",
     ARG_BOOLEAN,
     "no",
     "Transcribe audio from microphone."},
    {"-time",
     ARG_BOOLEAN,
     "no",
     "Print word times in file transcription."},
    CMDLN_EMPTY_OPTION
};

static ps_decoder_t *ps;
static cmd_ln_t *config;
static FILE *rawfd;

/* Sleep for specified msec */
static void
sleep_msec(int32 ms)
{
#if (defined(_WIN32) && !defined(GNUWINCE)) || defined(_WIN32_WCE)
    Sleep(ms);
#else
    /* ------------------- Unix ------------------ */
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;

    select(0, NULL, NULL, NULL, &tmo);
#endif
}

/*
 * Main utterance processing loop:
 *     for (;;) {
 *        start utterance and wait for speech to process
 *        decoding till end-of-utterance silence will be detected
 *        print utterance result;
 *     }
 */

ad_rec_t *ad;
int16 adbuf[2048];
uint8 utt_started, in_speech;
int32 k;
char const *hyp;

typedef struct nbest_s {
	ps_nbest_t *nbest;
} Nbest;



/*static void
recognize_from_microphone()
{
    ad_rec_t *ad;
    int16 adbuf[2048];
    uint8 utt_started, in_speech;
    int32 k;
    char const *hyp;

    if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
                          (int) cmd_ln_float32_r(config,
                                                 "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");

    if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");
    utt_started = FALSE;
    E_INFO("Ready....\n");

    for (;;) {
        if ((k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            E_INFO("Listening...\n");
        }
        if (!in_speech && utt_started) {

            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL) {
                printf("%s\n", hyp);
                fflush(stdout);
            }

            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");
            utt_started = FALSE;
            E_INFO("Ready....\n");
        }
        sleep_msec(10);
    }
    ad_close(ad);
}*/

int
main(int argc, char *argv[])
{
    err_set_logfp(NULL);

    char const *cfg;

    config = cmd_ln_parse_r(NULL, cont_args_def, argc, argv, TRUE);

    /* Handle argument file as -argfile. */
    if (config && (cfg = cmd_ln_str_r(config, "-argfile")) != NULL) {
        config = cmd_ln_parse_file_r(config, cont_args_def, cfg, FALSE);
    }

    if (config == NULL || (cmd_ln_str_r(config, "-infile") == NULL && cmd_ln_boolean_r(config, "-inmic") == FALSE)) {
	E_INFO("Specify '-infile <file.wav>' to recognize from file or '-inmic yes' to recognize from microphone.\n");
        cmd_ln_free_r(config);
	return 1;
    }

    ps_default_search_args(config);
    ps = ps_init(config);
    if (ps == NULL) {
        cmd_ln_free_r(config);
        return 1;
    }

    E_INFO("%s COMPILED ON: %s, AT: %s\n\n", argv[0], __DATE__, __TIME__);

    if (cmd_ln_str_r(config, "-infile") != NULL) {
        //recognize_from_file();
    } else if (cmd_ln_boolean_r(config, "-inmic")) {
        //recognize_from_microphone();
    }

    ps_free(ps);
    cmd_ln_free_r(config);

    return 0;
}

#if defined(_WIN32_WCE)
#pragma comment(linker,"/entry:mainWCRTStartup")
#include <windows.h>
//Windows Mobile has the Unicode main only
int
wmain(int32 argc, wchar_t * wargv[])
{
    char **argv;
    size_t wlen;
    size_t len;
    int i;

    argv = malloc(argc * sizeof(char *));
    for (i = 0; i < argc; i++) {
        wlen = lstrlenW(wargv[i]);
        len = wcstombs(NULL, wargv[i], wlen);
        argv[i] = malloc(len + 1);
        wcstombs(argv[i], wargv[i], wlen);
    }

    //assuming ASCII parameters
    return main(argc, argv);
}
#endif



/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRSphinxDevice.h"

namespace MinVR {

void
VRSphinxDevice::recognize_from_microphone()
{
	static int curCount;
	curCount++;
  //  for (;;) {
    	if ((k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");
        //std::cout << "frame" << std::endl;
    	//if ((curCount % 3) == 0) {
            ps_process_raw(ps, adbuf, k, FALSE, FALSE);
            in_speech = ps_get_in_speech(ps);
            //printf("Frame...\n");
            if (in_speech && !utt_started) {
                utt_started = TRUE;
                printf("Listening...\n");
            }
            if (!in_speech && utt_started) {
    //        	std::unique_lock<std::mutex> lock(counter_mutex);
                ps_end_utt(ps);
                Nbest *nbest = (Nbest*)ckd_calloc(1, sizeof(*nbest));
                nbest->nbest = ps_nbest(ps);
                nbest->nbest = ps_nbest_next(nbest->nbest);
                int32 score;
                int count = 0;

                std::vector<std::string> words;

                while (nbest->nbest != NULL && count < 10) {
                	hyp = ps_nbest_hyp(nbest->nbest, &score);

                	if (hyp != NULL) {
                		std::string str(hyp);
                		if (str.size() > 0) {
                    		words.push_back(hyp);
                		}
                	}

                	nbest->nbest = ps_nbest_next(nbest->nbest);
                	if (nbest->nbest != NULL) {
                    	hyp = ps_nbest_hyp(nbest->nbest, &score);
                	}
                	count++;

                	if (words.size() > 0) {
                		deviceMutex.lock();
                		dataIndex.addData("/Words", words);
                		events.push_back(dataIndex.serialize("/Words"));
                		deviceMutex.unlock();
                	}

                	//if ((k = ad_read(ad, adbuf, 2048)) < 0)
                		//E_FATAL("Failed to read audio\n");
                }

                if (nbest->nbest) {
                    ps_nbest_free(nbest->nbest);
                }

                ckd_free(nbest);
                /*hyp = ps_get_hyp(ps, NULL );
                if (hyp != NULL) {
                    printf("%s\n", hyp);
                    fflush(stdout);
                }*/

                if (ps_start_utt(ps) < 0)
                    E_FATAL("Failed to start utterance\n");
                utt_started = FALSE;
                printf("Ready....\n");
    	//}

        }
        sleep_msec(10);
    //}
    //ad_close(ad);
}

VRSphinxDevice::VRSphinxDevice() {
	deviceThread = new std::thread(&VRSphinxDevice::run, this);
}

VRSphinxDevice::~VRSphinxDevice() {
	// Join the thread if needed
	if (deviceThread) {
		deviceThread->join();
	}

	// Delete thread
	delete deviceThread;
}

void VRSphinxDevice::appendNewInputEventsSinceLastCall(
		VRDataQueue* inputEvents) {
	deviceMutex.lock();
    for (int f = 0; f < events.size(); f++)
    {
    	std::cout << events[f] << std::endl;
    	inputEvents->push(events[f]);
    }

    events.clear();
    deviceMutex.unlock();
}

void VRSphinxDevice::run() {
	while (true) {
		recognize_from_microphone();
	}
}

VRInputDevice* VRSphinxDevice::create(VRMainInterface* vrMain,
		MinVR::VRDataIndex* config2, const std::string& nameSpace) {
    err_set_logfp(NULL);

    char const *cfg;

    int argc = 5;


    char** argv =  (char**)malloc(argc * sizeof(char *));
    argv[0] = new char[0];
    argv[1] = new char[6];
    argv[2] = new char[3];
    argv[3] = new char[9];
    argv[4] = new char[2];
    std::strcpy(argv[1], "-inmic");
    std::strcpy(argv[2], "yes");
    std::strcpy(argv[3], "-bestpath");
    std::strcpy(argv[4], "no");


    config = cmd_ln_parse_r(NULL, cont_args_def, argc, argv, TRUE);

    /* Handle argument file as -argfile. */
    if (config && (cfg = cmd_ln_str_r(config, "-argfile")) != NULL) {
        config = cmd_ln_parse_file_r(config, cont_args_def, cfg, FALSE);
    }

    if (config == NULL || (cmd_ln_str_r(config, "-infile") == NULL && cmd_ln_boolean_r(config, "-inmic") == FALSE)) {
	E_INFO("Specify '-infile <file.wav>' to recognize from file or '-inmic yes' to recognize from microphone.\n");
        cmd_ln_free_r(config);
        return NULL;
    }

    ps_default_search_args(config);
    ps = ps_init(config);
    if (ps == NULL) {
        cmd_ln_free_r(config);
        return NULL;
    }

    E_INFO("%s COMPILED ON: %s, AT: %s\n\n", argv[0], __DATE__, __TIME__);

    if (cmd_ln_str_r(config, "-infile") != NULL) {
        //recognize_from_file();
    } else if (cmd_ln_boolean_r(config, "-inmic")) {
        //recognize_from_microphone();
    }

    if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
                          (int) cmd_ln_float32_r(config,
                                                 "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");

    if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");
    utt_started = FALSE;
    E_INFO("Ready....\n");

    //ps_free(ps);
    //cmd_ln_free_r(config);

	return new VRSphinxDevice();
}

} /* namespace MinVR */
