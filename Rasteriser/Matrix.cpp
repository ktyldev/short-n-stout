#include "Matrix.h"
#include <cmath>

Matrix::Matrix() 
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			_m[i][j] = 0.0f;
		}
	}
}

// Constructor that initialises all elements.  Note that an initialiser list is used
// to pass in the values.  This is a new C++ feature.
Matrix::Matrix(std::initializer_list<float> list) 
{
	// Check that the initialiser list is the same size as the 
	// size of the array
	if (list.size() != ROWS * COLS)
	{
		// If not, throw an exception
		throw "Insufficient values supplied";
	}
	// Use iterator to work through the initialiser list, 
	// retrieving each value in turn.
	auto iterator = list.begin();
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			_m[i][j] = *iterator++;
		}
	}
}

// Copy constructor
Matrix::Matrix(const Matrix& other)
{
	Copy(other);
}

Matrix::Matrix(Matrix&& other) 
{
	Copy(other);
}

// Destructor
Matrix::~Matrix()
{
}

// Retrieve value in matrix at specified row and column
float Matrix::GetM(const int row, const int column) const
{
	return _m[row][column];
}

// Set value in matrix at specified row and column
void Matrix::SetM(const int row, const int column, const float value)
{
	_m[row][column] = value;
}

// Copy assignment operator
Matrix& Matrix::operator= (const Matrix &rhs)
{
	if (this != &rhs)
	{
		// Only do if not assigning to ourselves (remember that a = b is the same as a.operator=(b))
		Copy(rhs);
	}
	return *this;
}

// Move assignment operator
Matrix& Matrix::operator= (Matrix &&rhs)
{
	if (this != &rhs)
	{
		// Only do if not assigning to ourselves (remember that a = b is the same as a.operator=(b))
		Copy(rhs);
	}
	return *this;
}

// Test for two matrices being equivalent
bool Matrix::operator==(const Matrix &other) const
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (_m[i][j] != other.GetM(i, j))
			{
				return false;
			}
		}
	}
	return true;
}

// Multiply two matrices together
const Matrix Matrix::operator*(const Matrix &other) const
{
	Matrix result;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			float resultValue = 0;
			for (int k = 0; k < ROWS; k++)
			{
				resultValue += _m[i][k] * other.GetM(k, j);
			}
			result.SetM(i, j, resultValue);
		}
	}
	return result;
}

const Vertex Matrix::operator*(const Vertex &p) const
{
	Vertex newVertex(p);

	newVertex.SetX(_m[0][0] * p.GetX() + _m[0][1] * p.GetY() + _m[0][2] * p.GetZ() + _m[0][3] * p.GetW());
	newVertex.SetY(_m[1][0] * p.GetX() + _m[1][1] * p.GetY() + _m[1][2] * p.GetZ() + _m[1][3] * p.GetW());
	newVertex.SetZ(_m[2][0] * p.GetX() + _m[2][1] * p.GetY() + _m[2][2] * p.GetZ() + _m[2][3] * p.GetW());
	newVertex.SetW(_m[3][0] * p.GetX() + _m[3][1] * p.GetY() + _m[3][2] * p.GetZ() + _m[3][3] * p.GetW());
	return newVertex;
}

Matrix Matrix::IdentityMatrix()
{
	return Matrix{ 1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1 };
}

Matrix Matrix::TranslationMatrix(float x, float y, float z)
{
	return Matrix{ 1, 0, 0, x,
				  0, 1, 0, y,
				  0, 0, 1, z,
				  0, 0, 0, 1 };
}

Matrix Matrix::ScalingMatrix(float x, float y, float z)
{
	return Matrix{ x, 0, 0, 0,
				  0, y, 0, 0,
				  0, 0, z, 0,
				  0, 0, 0, 1 };
}

Matrix Matrix::XRotationMatrix(float angle)
{
	return Matrix{ 1, 0,         0,			  0,
				  0, cos(angle), -sin(angle), 0,
				  0, sin(angle), cos(angle),  0,
				  0, 0,			 0,			  1};
}

Matrix Matrix::YRotationMatrix(float angle)
{
	return Matrix{ cos(angle),  0, sin(angle), 0,
				   0,			1, 0,		   0,
				   -sin(angle), 0, cos(angle), 0,
				   0,			0, 0,		   1 };
}

Matrix Matrix::ZRotationMatrix(float angle)
{
	return Matrix{ cos(angle), -sin(angle), 0, 0,
				   sin(angle), cos(angle),  0, 0,
				   0,		   0,		    1, 0,
				   0,		   0,			0, 1 };
}

Matrix Matrix::XYZRotationMatrix(float angleX, float angleY, float angleZ)
{
	return XRotationMatrix(angleX) * YRotationMatrix(angleY) * ZRotationMatrix(angleZ);
}




// Private method to copy contents of one matrix
// to another
void Matrix::Copy(const Matrix & other)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			_m[i][j] = other.GetM(i, j);
		}
	}
}

