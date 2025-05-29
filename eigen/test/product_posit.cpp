#include "product.h"
#include <Eigen/LU>
#include "../../soft-posit-cpp/include/softposit_cpp.h"
#include <cstring> // For memcpy

// Using Eigen types
using Eigen::Matrix;
using Eigen::Dynamic;
using Eigen::MatrixXd;

// Helper functions for posit conversions
posit32_t double_to_posit(double val) {
    posit32_t p;
    float temp = static_cast<float>(val);
    std::memcpy(&p, &temp, sizeof(float));
    return p;
}

double posit_to_double(posit32_t p) {
    float temp;
    std::memcpy(&temp, &p, sizeof(float));
    return static_cast<double>(temp);
}

// Regression test for bug 447 with posit support
template <int>
void product1x1() {
  // Original float tests
  {
    Matrix<float, 1, 3> matAstatic;
    Matrix<float, 3, 1> matBstatic;
    matAstatic.setRandom();
    matBstatic.setRandom();
    VERIFY_IS_APPROX((matAstatic * matBstatic).coeff(0, 0), 
                    matAstatic.cwiseProduct(matBstatic.transpose()).sum());
  }

  // Posit tests
  {
    Matrix<posit32_t, 1, 3> matAstatic;
    Matrix<posit32_t, 3, 1> matBstatic;
    
    // Initialize with posit values
    for (int i = 0; i < 3; i++) {
        matAstatic(0,i) = double_to_posit(i+1);
        matBstatic(i,0) = double_to_posit(i+1);
    }
    
    posit32_t product = (matAstatic * matBstatic).coeff(0, 0);
    posit32_t sum = matAstatic.cwiseProduct(matBstatic.transpose()).sum();
    
    // Convert to double for comparison
    VERIFY_IS_APPROX(posit_to_double(product), posit_to_double(sum));
  }
}

// Reference product implementation for posits
template <typename TC, typename TA, typename TB>
const TC &ref_prod(TC &C, const TA &A, const TB &B) {
  for (Index i = 0; i < C.rows(); ++i) {
    for (Index j = 0; j < C.cols(); ++j) {
      double sum = 0.0;
      for (Index k = 0; k < A.cols(); ++k) {
        sum += posit_to_double(A.coeff(i, k)) * posit_to_double(B.coeff(k, j));
      }
      C.coeffRef(i, j) = double_to_posit(sum);
    }
  }
  return C;
}

// Posit accuracy test
void test_posit_accuracy() {
  Matrix<posit32_t, 3, 3> A_posit;
  Matrix<float, 3, 3> A_float;
  
  // Initialize matrices
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A_posit(i,j) = double_to_posit(1.0);
      A_float(i,j) = 1.0f;
    }
  }
  A_posit(1,1) = double_to_posit(1.0001);
  A_posit(2,2) = double_to_posit(1.0001);
  A_float(1,1) = 1.0001f;
  A_float(2,2) = 1.0001f;
  
  Matrix<posit32_t, 3, 1> b_posit;
  Matrix<float, 3, 1> b_float;
  for (int i = 0; i < 3; i++) {
    b_posit(i) = double_to_posit(3.0 + (i > 0 ? 0.0001 : 0.0));
    b_float(i) = 3.0f + (i > 0 ? 0.0001f : 0.0f);
  }
  
  // Solve and compare
  auto x_posit = A_posit.partialPivLu().solve(b_posit);
  auto x_float = A_float.partialPivLu().solve(b_float);
  
  // Calculate residuals as matrices first
  auto resid_posit_mat = A_posit * x_posit - b_posit;
  auto resid_float_mat = A_float * x_float - b_float;
  
  // Convert to double through our conversion function
  double resid_posit = 0.0;
  for (int i = 0; i < 3; i++) {
    resid_posit += posit_to_double(resid_posit_mat(i)) * posit_to_double(resid_posit_mat(i));
  }
  resid_posit = std::sqrt(resid_posit);
  
  double resid_float = resid_float_mat.norm();
  
  std::cout << "Posit residual: " << resid_posit << "\n";
  std::cout << "Float residual: " << resid_float << "\n";
  
  // Verification
  VERIFY_IS_APPROX(resid_posit, resid_float);
}

EIGEN_DECLARE_TEST(product_small) {
  for (int i = 0; i < g_repeat; i++) {
    // Original tests
    CALL_SUBTEST_1(product(Matrix<float, 3, 2>()));
    
    // Posit tests
    CALL_SUBTEST_101(product(Matrix<posit32_t, 3, 2>()));
    CALL_SUBTEST_102(test_posit_accuracy());
  }
}