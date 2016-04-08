/** This small math library provides lightweight support for the
    graphics math needed inside MinVR. Some aspects (e.g., separate
    classes for Point3 and Vector3) are inspired the math libraries
    used in Brown computer graphics courses.  Also based on some
    routines introduced in the Hill & Kelley text used in UMN courses.
    Intended to be lightweight, for use inside MinVR only since
    application programmers will probably want to use the math package
    that is native to whatever graphics engine they are pairing with
    MinVR.
*/

#ifndef VRMATH_H
#define VRMATH_H

#include <iostream>

#include <config/VRDatum.h>


/** @class VRPoint3 
  * @brief 3D Point with floating point coordinates.
  */
class VRPoint3 : public VRDoubleArrayConvertible {
public:  
  /// Default point at the origin
  VRPoint3();

  /// Constructs a point given (x,y,z, 1)
  VRPoint3(double x, double y, double z);

  /// Constructs a point given a pointer to x,y,z data
  VRPoint3(double *p);
  
  /// Constructs a point from a VRDoubleArray with the first three elements
  /// being [x,y,z]
  VRPoint3(VRDoubleArray da);
  
  /// Constructs a point from the VRAnyCoreType wrapper class. The argument
  /// must be able to be interpreted as a VRDoubleArray core type.
  VRPoint3(VRAnyCoreType t);
  
  /// Copy constructor for point
  VRPoint3(const VRPoint3& p);

  /// Point destructor
  virtual ~VRPoint3();
  
  /// Check for "equality", taking floating point imprecision into account
  bool operator==(const VRPoint3& p) const;

  /// Check for "inequality", taking floating point imprecision into account
  bool operator!=(const VRPoint3& p) const;

  /// Assignment operator
  VRPoint3& operator=(const VRPoint3& p);

  /// Accesses the ith coordinate of the point
  double operator[](const int i) const;

  /// Accesses the ith coordinate of the point
  double& operator[](const int i);
  
  /// Converts the point to a VRDoubleArray for data in a VRDataIndex
  VRDoubleArray toVRDoubleArray();

public:
  double x,y,z; 
};




/** @class VRVector3 
  * @brief 3D vector (magnitude and direction).
  */
class VRVector3 : public VRDoubleArrayConvertible {
public:
  /// Default constructor to create zero vector
  VRVector3();

  /// Constructs a vector (x, y, z, 0)
  VRVector3(double x, double y, double z);

  /// Constructs a vector given a pointer to x,y,z data
  VRVector3(double *v);
  
  /// Constructs a vector from a VRDoubleArray with the first three elements
  /// being [x,y,z]
  VRVector3(VRDoubleArray da);
  
  /// Constructs a vector from the VRAnyCoreType wrapper class. The argument
  /// must be able to be interpreted as a VRDoubleArray core type.
  VRVector3(VRAnyCoreType t);

  /// Copy constructor for vector
  VRVector3(const VRVector3& v);

  /// Vector destructor
  virtual ~VRVector3();

  /// Check for "equality", taking floating point imprecision into account
  bool operator==(const VRVector3& v) const;

  /// Check for "inequality", taking floating point imprecision into account
  bool operator!=(const VRVector3& v) const;

  /// Vector assignment operator
  VRVector3& operator=(const VRVector3& v);

  /// Returns the ith coordinate of the vector
  double operator[](const int i) const;

  /// Returns the ith coordinate of the vector
  double& operator[](const int i);  

  // --- Vector operations ---

  /// Returns "this dot v"
  double dot(const VRVector3& v) const;

  /// Returns "this cross v"
  VRVector3 cross(const VRVector3& v) const;

  /// Returns the length of the vector
  double length() const;

  /// Returns a normalized (i.e. unit length) version of the vector
  VRVector3 normalize();
  
  /// Converts the point to a VRDoubleArray for data in a VRDataIndex
  VRDoubleArray toVRDoubleArray();

public:
  double x,y,z; 
};



/** @class VRMatrix4
  * @brief A 4x4 transformation matrix
  */
class VRMatrix4 : public VRDoubleArrayConvertible {
public: 
  /// Default constructor creates an identity matrix:
  VRMatrix4();

  /// Constructs a matrix given the elments.  Be careful, the elements are 
  /// passed in using row major order, so the matrix looks "correct" on the
  /// screen if you write this constructor using 4 lines as it is below.  But,
  /// to stay consistent with OpenGL, the matrix is stored internally in
  /// column major order!
  VRMatrix4(const double r1c1, const double r1c2, const double r1c3, const double r1c4, 
            const double r2c1, const double r2c2, const double r2c3, const double r2c4,
            const double r3c1, const double r3c2, const double r3c3, const double r3c4, 
            const double r4c1, const double r4c2, const double r4c3, const double r4c4);

  /// Constructs a matrix given a 16 element array
  VRMatrix4(const double* a);
  
  /// Constructs a matrix from a VRDoubleArray with 16 elements
  VRMatrix4(VRDoubleArray da);
  
  /// Constructs a matrix from the VRAnyCoreType wrapper class. The argument
  /// must be able to be interpreted as a VRDoubleArray core type.
  VRMatrix4(VRAnyCoreType t);

  /// Copy constructor
  VRMatrix4(const VRMatrix4& m2);

  /// Destructor
  virtual ~VRMatrix4();
  
  /// Check for "equality", taking floating point imprecision into account
  bool operator==(const VRMatrix4& m2) const;

  /// Check for "inequality", taking floating point imprecision into account
  bool operator!=(const VRMatrix4& m2) const;

  /// Matrix assignment operator
  VRMatrix4& operator=(const VRMatrix4& m2);
  
  /// Returns the (r,c) elment of the matrix
  double operator()(const int r, const int c) const;
  
  /// Returns the (r,c) elment of the matrix
  double& operator()(const int r, const int c);


  // --- Static Constructors for Special Matrices ---

  /// Returns the scale matrix described by the vector
  static VRMatrix4 scale(const VRVector3& v);

  /// Returns the translation matrix described by the vector
  static VRMatrix4 translation(const VRVector3& v);

  /// Returns the rotation matrix about the x axis by the specified angle
  static VRMatrix4 rotationX(const double radians);

  /// Returns the rotation matrix about the y axis by the specified angle
  static VRMatrix4 rotationY(const double radians);

  /// Returns the rotation matrix about the z axis by the specified angle
  static VRMatrix4 rotationZ(const double radians);
  
  /// Returns the rotation matrix around the vector v placed at point p, rotate by angle a
  static VRMatrix4 rotation(const VRPoint3& p, const VRVector3& v, const double a);


  // --- Transpose, Inverse, and Other General Matrix Functions ---

  /// Returns transpose of the matrix
  VRMatrix4 transpose();

  // Returns the determinant of the 3x3 matrix formed by excluding the specified row and column
  // from the 4x4 matrix.
  double subDeterminant(int excludeRow, int excludeCol);

  // Returns the cofactor matrix.
  VRMatrix4 cofactor();

  // Returns the determinant of the 4x4 matrix
  double determinant();

  // Returns the inverse of the 4x4 matrix if it is nonsingular.  If it is singular, then returns the
  // identity matrix. 
  VRMatrix4 inverse();
  
  /// Converts the point to a VRDoubleArray for data in a VRDataIndex
  VRDoubleArray toVRDoubleArray();

  // Gets the double array
  const double* getArray() { return m; }

public:		
  double m[16]; // hold a 4 by 4 matrix 
};



// ---------- Operator Overloads for Working with Points, Vectors, & Matrices ---------- 


// --- Scalers ---

/// Divide the vector by the scalar s
VRVector3 operator/(const VRVector3& v, const double s);

/// Multiply the vector by the scalar s
VRVector3 operator*(const double s, const VRVector3& v);

/// Multiply the vector by the scalar s
VRVector3 operator*(const VRVector3& v, const double s);

/// Negate the vector
VRVector3 operator-(const VRVector3& v);

// Note: no -(point) operator, that's an undefined operation


// --- Point and Vector Arithmetic ---

/// Adds a vector and a point, returns a point
VRPoint3 operator+(const VRVector3& v, const VRPoint3& p);

/// Adds a point and a vector, returns a point
VRPoint3 operator+(const VRPoint3& p, const VRVector3& v);

/// Adds a vector and a vector, returns a vector
VRVector3 operator+(const VRVector3& v1, const VRVector3& v2);

// Note: no (point + point) operator, that's an undefined operation

/// Subtracts a vector from a point, returns a point
VRPoint3 operator-(const VRPoint3& p, const VRVector3& v);

/// Subtracts v2 from v1, returns a vector
VRVector3 operator-(const VRVector3& v1, const VRVector3& v2);

/// Returns the vector spanning p1 and p2
VRVector3 operator-(const VRPoint3& p1, const VRPoint3& p2);

// Note: no (vector - point) operator, that's an undefined operation


// --- Matrix multiplication for Points, Vectors, & Matrices ---

/// Multiply matrix and scalar, returns the new matrix
VRMatrix4 operator*(const VRMatrix4& m, const double& s);

/// Multiply matrix and scalar, returns the new matrix
VRMatrix4 operator*(const double& s, const VRMatrix4& m);

/// Multiply matrix and point, returns the new point
VRPoint3 operator*(const VRMatrix4& m, const VRPoint3& p);

/// Multiply matrix and vector, returns the new vector
VRVector3 operator*(const VRMatrix4& m, const VRVector3& v);

/// Multiply two matrices, returns the result
VRMatrix4 operator*(const VRMatrix4& m1, const VRMatrix4& m2);


// --- Stream operators ---

// VRPoint3
std::ostream & operator<< ( std::ostream &os, const VRPoint3 &p);
std::istream & operator>> ( std::istream &is, VRPoint3 &p);

// VRVector3
std::ostream & operator<< ( std::ostream &os, const VRVector3 &v);
std::istream & operator>> ( std::istream &is, VRVector3 &v);

// VRMatrix4
std::ostream & operator<< ( std::ostream &os, const VRMatrix4 &m);
std::istream & operator>> ( std::istream &is, VRMatrix4 &m);

#endif
