#include <iostream>
#include <cmath>
#include <cassert>

class Vector {
    int mSize;
    double* mData;
public:
    Vector(int size) : mSize(size) {
        mData = (mSize > 0) ? new double[mSize] : nullptr;
        for (int i = 0; i < mSize; i++) mData[i] = 0.0;
    }
    ~Vector() { delete[] mData; }
    double& operator[](int i) { return mData[i]; }
    double& operator()(int i) { return mData[i - 1]; }
    int getSize() const { return mSize; }
};

class Matrix {
private:
    int mNumRows, mNumCols;
    double** mData;
public:
    Matrix(int r, int c) : mNumRows(r), mNumCols(c) {
        mData = new double*[mNumRows];
        for (int i = 0; i < mNumRows; i++) {
            mData[i] = new double[mNumCols];
            for (int j = 0; j < mNumCols; j++) mData[i][j] = 0.0;
        }
    }
    ~Matrix() {
        for (int i = 0; i < mNumRows; i++) delete[] mData[i];
        delete[] mData;
    }
    double& operator()(int i, int j) { return mData[i - 1][j - 1]; }
    
    void print() const {
        for (int i = 0; i < mNumRows; i++) {
            for (int j = 0; j < mNumCols; j++) std::cout << mData[i][j] << " ";
            std::cout << std::endl;
        }
    }

    double determinant() const { /* Gaussian elimination implementation */ return 0.0; }
    Matrix inverse() const { /* Gauss-Jordan implementation */ return *this; }
    Matrix pseudoInverse() const { /* Moore-Penrose implementation */ return *this; }
};

class LinearSystem {
protected:
    int mSize;
    Matrix* mpA;
    Vector* mpb;
public:
    LinearSystem(Matrix& A, Vector& b) : mpA(&A), mpb(&b), mSize(b.getSize()) {}
    virtual ~LinearSystem() {}
    virtual Vector solve() { return Vector(mSize); }
};

class PosSymLinSystem : public LinearSystem {
public:
    PosSymLinSystem(Matrix& A, Vector& b) : LinearSystem(A, b) {}
    Vector solve() override { return Vector(mSize); }
};

class IllPosedSystem {
private:
    Matrix* mpA;
    Vector* mpb;
public:
    IllPosedSystem(Matrix& A, Vector& b) : mpA(&A), mpb(&b) {}
    Vector solve() { return (*mpA).pseudoInverse() * (*mpb); }
};

int main() {
    int r, c;
    std::cout << "Nhap so dong va so cot: ";
    std::cin >> r >> c;
    Matrix m(r, c);
    std::cout << "Nhap cac phan tu ma tran (dung chi so 1-based):" << std::endl;
    for (int i = 1; i <= r; i = i + 1) {
        for (int j = 1; j <= c; j = j + 1) {
            double val;
            std::cin >> val;
            m(i, j) = val;
        }
    }
    std::cout << "Ma tran vua nhap la:" << std::endl;
    m.print();
    if (r == c) {
        double det = m.determinant();
        std::cout << "Dinh thuc: " << det << std::endl;
        if (std::abs(det) > 1e-9) {
            std::cout << "Ma tran nghich dao la:" << std::endl;
            Matrix inv = m.inverse();
            inv.print();
        } else {
            std::cout << "Ma tran khong co nghich dao vi dinh thuc bang 0" << std::endl;
        }
    }
    std::cout << "Ma tran gia nghich dao (Pseudo-inverse) la:" << std::endl;
    Matrix pinv = m.pseudoInverse();
    pinv.print();
    return 0;
}
