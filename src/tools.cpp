// #include <iostream>
// #include "tools.h"

// using Eigen::VectorXd;
// using Eigen::MatrixXd;
// using std::vector;

// Tools::Tools() {}

// Tools::~Tools() {}

// VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
//                               const vector<VectorXd> &ground_truth) {
//   /**
//   TODO:
//     * Calculate the RMSE here.
//   */
//   VectorXd rmse(4);
// 	rmse << 0,0,0,0;

//   if(estimations.size() != ground_truth.size()
// 			|| estimations.size() == 0){
// 		cout << "Invalid estimation or ground_truth data" << endl;
// 		return rmse;
// 	}

//   //accumulate squared residuals
// 	for(unsigned int i=0; i < estimations.size(); ++i){

// 		VectorXd residual = estimations[i] - ground_truth[i];

// 		//coefficient-wise multiplication
// 		residual = residual.array()*residual.array();
// 		rmse += residual;
// 	}

//   //calculate the mean
// 	rmse = rmse/estimations.size();
// 	rmse = rmse.array().sqrt();

// 	return rmse;
// }

// MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
//   /**
//   TODO:
//     * Calculate a Jacobian here.
//   */
//   MatrixXd Hj(3,4);
// 	//recover state parameters
// 	float px = x_state(0);
// 	float py = x_state(1);
// 	float vx = x_state(2);
// 	float vy = x_state(3);
//   if (fabs(px) < 0.0001 and fabs(py) < 0.0001){
// 	  px = 0.0001;
// 	  py = 0.0001;
//   }
//   //pre-compute a set of terms to avoid repeated calculation
// 	float c1 = px*px+py*py;
//   c1 = (fabs(c1) < 0.0000001) ? 0.0000001 : c1;
// 	float c2 = sqrt(c1);
// 	float c3 = (c1*c2);
//   //compute the Jacobian matrix
// 	Hj << (px/c2), (py/c2), 0, 0,
// 		  -(py/c1), (px/c1), 0, 0,
// 		  py*(vx*py - vy*px)/c3, px*(px*vy - py*vx)/c3, px/c2, py/c2;
// 	return Hj;
// }









#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
	VectorXd rmse(4);
	rmse << 0,0,0,0;

	if(estimations.size() == 0 || ground_truth.size() == 0){
		cout << "ERROR - CalculateRMSE ()" << endl;
		return rmse;
	}

	unsigned int n = estimations.size();
	if(n != ground_truth.size()){
		cout << "ERROR - CalculateRMSE () - The ground-truth and estimations vectors must have the same size." << endl;
		return rmse;
	}

	for(unsigned int i=0; i < estimations.size(); ++i){

		VectorXd residual = estimations[i] - ground_truth[i];

		//coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
	}

	//calculate the mean
	rmse = rmse/n;
	rmse = rmse.array().sqrt();

	cout << "rmse = " << rmse << endl;
	return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  MatrixXd Hj(3,4);

  if ( x_state.size() != 4 ) {
    cout << "ERROR - CalculateJacobian ()" << endl;
    return Hj;
  }
	//recover state parameters
	double px = x_state(0);
	double py = x_state(1);
	double vx = x_state(2);
	double vy = x_state(3);

	//pre-compute a set of terms to avoid repeated calculation
	double c1 = px*px+py*py;
	double c2 = sqrt(c1);
	double c3 = (c1*c2);

	//check division by zero
	if(fabs(c1) < 0.0001){
		cout << "ERROR - CalculateJacobian ()" << endl;
		return Hj;
	}

	//compute the Jacobian matrix
	Hj << (px/c2), (py/c2), 0, 0,
		  -(py/c1), (px/c1), 0, 0,
		  py*(vx*py - vy*px)/c3, px*(px*vy - py*vx)/c3, px/c2, py/c2;

	return Hj;
}
