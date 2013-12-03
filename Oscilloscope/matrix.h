#pragma once

#ifndef _MATRIX_H
#define _MATRIX_H
#endif 

#include "pw_common.h"
template<typename T>
class Matrix
{
public:
	Matrix();
	Matrix(int row);
	Matrix(int row,int col);
	Matrix(const Matrix<T> &m);
	Matrix(T** m, int row, int col, bool Static = false);
	Matrix(T* m, int row, int col);
	Matrix(T* m, int col);
	virtual ~Matrix();

	void SetSize(int row);
	void SetSize(int row,int col);		 
	void SetUnit();
	void SetValue(const T& d);
	int GetRow() const;
	int GetCol() const;

	Matrix<T> Transpose() const;
	Matrix<T> operator+(const Matrix<T>& rhs) const;
	Matrix<T> operator-(const Matrix<T>& rhs) const;
	Matrix<T> operator*(const Matrix<T>& rhs) const;
	Matrix<T> operator/(const Matrix<T>& rhs) const;
	Matrix<T> operator*(const T& d) const;
	Matrix<T> operator/(const T& d) const;
	Matrix<T>& operator=(const Matrix<T>& m);
	Matrix<T> Inverse() const;
	bool IsSquare() const; 
	bool IsSymmetric() const;
	T ToValue() const;
	T& At(int i, int j);
	T At(int i, int j) const;
	virtual void Print() const;
	void ToFile(string file = "matrixdebug.log") const;

protected:
	void Release();
	Matrix<T> Exchange(int i,int j);
	Matrix<T> Multiple(int index, const T& mul);                  
	Matrix<T> MultipleAdd(int index,int src, const T& mul);
	int Pivot(int row) const;

private:
	T** m_data;
	int Row, Col;

};


template<typename T>
Matrix<T>::Matrix()
{
	Row = 0;
	Col = 0;	
	m_data = NULL;
}

template<typename T>
Matrix<T>::Matrix(int row)
{
	Row = row;
	Col = row;

	m_data = new T*[row];
	for (int i=0; i<Row; i++)
	{
		m_data[i] = new T[Col];
	}

}

template<typename T>
Matrix<T>::Matrix(int row,int col)
{
	Row = row;
	Col = col;

	m_data = new T*[row];
	for (int i=0; i<Row; i++)
	{
		m_data[i] = new T[Col];
	}
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> &m)
{
	Row = m.Row;
	Col = m.Col;

	m_data = new T*[Row];
	for (int i=0; i<Row; i++)
	{
		m_data[i] = new T[Col];
		for(int j=0;j<Col;j++)
		{
			m_data[i][j] = m.m_data[i][j];
		}

	}
}

template<typename T>
Matrix<T>::Matrix(T* m, int row, int col)
{
	Row = row;
	Col = col;

	m_data = new T*[Row];
	for (int i=0; i<Row; i++)
	{
		m_data[i] = new T[Col];
		for(int j=0;j<Col;j++)
		{
			m_data[i][j] = *((T*)m+i*Col+j);
		}

	}
}

template<typename T>
Matrix<T>::Matrix(T** m, int row, int col, bool Static)
{
	Row = row;
	Col = col;

	m_data = new T*[Row];
	for (int i=0; i<Row; i++)
	{
		m_data[i] = new T[Col];
		for(int j=0;j<Col;j++)
		{
			if (Static)
				m_data[i][j] = *((T*)m+i*Col+j);
			else
				m_data[i][j] = m[i][j];

		}

	}
}

template<typename T>
Matrix<T>::Matrix(T* m, int col)
{
	Row = 1;
	Col = col;

	m_data = new T*[Row];	
	m_data[0] = new T[Col];
	for(int j=0;j<Col;j++)
	{
		m_data[0][j] = m[j];

	}
}

template<typename T>
Matrix<T>::~Matrix()
{
	Release();
}

template<typename T>
void Matrix<T>::Release()
{
	if (m_data != NULL)
	{
		for (int i=0; i<Row; i++)
			delete [] m_data[i];
		delete [] m_data;
		m_data = NULL;
	}
}

template<typename T>
void Matrix<T>::SetSize(int row)
{
	Release();		
	Row = row;
	Col = row;

	m_data = new T*[row];
	for (int i=0; i<Row; i++)
	{
		m_data[i] = new T[Col];
	}
}

template<typename T>
void Matrix<T>::SetSize(int row, int col)
{
	Release();		
	Row = row;
	Col = col;

	m_data = new T*[row];
	for (int i=0; i<Row; i++)
	{
		m_data[i] = new T[Col];
	}
}

template<typename T>		 
void Matrix<T>::SetUnit()
{
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
			m_data[i][j] = ((i==j)?1:0);
}

template<typename T>
void Matrix<T>::SetValue(const T& d)
{
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
			m_data[i][j] = d;
}

template<typename T>
int Matrix<T>::GetRow() const
{
	return Row;
}

template<typename T>
int Matrix<T>::GetCol() const
{
	return Col;
}

template<typename T>
Matrix<T> Matrix<T>::Exchange(int i,int j)
{
	T temp;

	for(int k=0;k<Col;k++)
	{
		temp = m_data[i][k];
		m_data[i][k] = m_data[j][k];
		m_data[j][k] = temp;
	}
	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::Multiple(int index, const T& mul)    
{
	for(int j=0;j<Col;j++)
	{
		m_data[index][j] *= mul;
	}
	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::MultipleAdd(int index, int src, const T& mul)
{
	for(int j=0;j<Col;j++)
	{
		m_data[index][j] += m_data[src][j]*mul;
	}

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::Transpose() const
{
	Matrix<T> ret(Col,Row);

	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
		{
			ret.m_data[j][i] = m_data[i][j];
		}
		return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs) const
{
	assert(Row == rhs.Row && Col == rhs.Col);

	Matrix<T> ret(Row, Col);

	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
		{
			ret[i][j] = m_data[i][j] + rhs.m_data[i][j];
		}
		return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs) const
{
	assert(Row == rhs.Row && Col == rhs.Col);

	Matrix<T> ret(Row, Col);

	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
		{
			ret.m_data[i][j] = m_data[i][j] - rhs.m_data[i][j];
		}
		return ret;

}

template<typename T> 
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) const
{
	if (Col != rhs.Row)
	{
		throw _Exception("Col != Row(In operator*)");
	}

	Matrix<T> ret(Row, rhs.Col);
	T temp;
	for(int i=0;i<Row;i++)
	{
		for(int j=0;j<rhs.Col;j++)
		{
			temp = 0;
			for(int k=0;k<Col;k++)
			{
				temp += m_data[i][k] * rhs.m_data[k][j];
			}
			ret.m_data[i][j] = temp;
		}
	}

	return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const Matrix<T>& rhs) const
{
	return *this * rhs.Inverse();
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T& d) const
{
	Matrix<T> ret(*this);
	for(int i=0;i<ret.Row;i++)
		for(int j=0;j<ret.Col;j++)
			ret.m_data[i][j] *= d;
	return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const T& d) const
{
	return d*this->Inverse();
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
	Release();
	Row = m.Row;
	Col = m.Col;

	m_data = new T*[Row];
	for (int i=0; i<Row; i++)
	{
		m_data[i] = new T[Col];
		for(int j=0;j<Col;j++)
		{
			m_data[i][j] = m.m_data[i][j];
		}

	}
	return *this;
}

template<typename T>
int Matrix<T>::Pivot(int row) const
{
	int index=row;

	for(int i=row+1;i<Row;i++)
	{
		if(abs(m_data[i][row]) > abs(m_data[index][row]))
			index=i;
	}

	return index;
}

template<typename T>
Matrix<T> Matrix<T>::Inverse() const
{
	if (Row != Col)
	{
		throw _Exception("Col != Row(In Inverse)");
	}

	Matrix<T> tmp(*this);
	Matrix<T> ret(Row);    
	ret.SetUnit();

	int maxIndex;
	T dMul;

	for(int i=0;i<Row;i++)
	{
		maxIndex = tmp.Pivot(i);
		if (tmp.m_data[maxIndex][i] == T(0) )
		{
			throw _Exception("0 error(In Inverse)");
		}


		if(maxIndex != i)  
		{
			tmp.Exchange(i,maxIndex);
			ret.Exchange(i,maxIndex);
		}

		ret.Multiple(i,T(1)/tmp.m_data[i][i]);
		tmp.Multiple(i,T(1)/tmp.m_data[i][i]);

		for(int j=i+1;j<Row;j++)
		{
			dMul = -tmp.m_data[j][i]/tmp.m_data[i][i];
			tmp.MultipleAdd(j,i,dMul);
			ret.MultipleAdd(j,i,dMul);

		}

	}

	for(int i=Row-1;i>0;i--)
	{
		for(int j=i-1;j>=0;j--)
		{
			dMul = -tmp.m_data[j][i]/tmp.m_data[i][i];
			tmp.MultipleAdd(j,i,dMul);
			ret.MultipleAdd(j,i,dMul);
		}
	}       
	return ret;
}

template<typename T>
bool Matrix<T>::IsSquare() const
{
	return Row==Col;
}

template<typename T>
bool Matrix<T>::IsSymmetric() const
{

	if(Row != Col) 
		return false;

	for(int i=0;i<Row;i++)
		for(int j=i+1;j<Col;j++)
			if( m_data[i][j] != m_data[j][i])
				return false;

	return true;
}

template<typename T>
T Matrix<T>::ToValue() const
{
	if (Row != 1 || Col != 1)
	{
		throw _Exception("Row != 1 or Col != 1(In ToValue)");
	}
	return m_data[0][0];
}

template<typename T>
T& Matrix<T>::At(int i, int j)
{
	if (i >= Row || j >= Col)
		throw _Exception("i >= Row or j >= Col(In At)");
	return m_data[i][j];
}

template<typename T>
T Matrix<T>::At(int i, int j) const
{
	if (i >= Row || j >= Col)
		throw _Exception("i >= Row or j >= Col(In At)");
	return m_data[i][j];
}
template<typename T>
void Matrix<T>::Print() const
{	
	for(int i=0;i<Row;i++)
	{
		for(int j=0;j<Col;j++)
		{
			cout << m_data[i][j] << ',';
		}
		cout << "\r\n";
	}
	cout << "\r\n";

}

template<typename T>
void Matrix<T>::ToFile(string file) const
{
	fstream fp;
	fp.open(file.c_str(), ios::out | ios::app);
	for(int i=0;i<Row;i++)
	{
		for(int j=0;j<Col;j++)
		{
			fp << m_data[i][j] << ',';
		}
		fp << "\r\n";
	}
	fp << "\r\n";
	fp.flush();
	fp.clear();
	fp.close();
}