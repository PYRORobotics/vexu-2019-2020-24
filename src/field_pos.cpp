#include "main.h"

pros::ADIEncoder left(3,4,false);

pros::ADIEncoder right(5,6,true);

pros::ADIGyro gyro(1);


double X_Pos = 0;
double Y_Pos = 0;
double X_saved = 0;
double Y_saved = 0;
int previousAngle;
int currentAngle;
int previousLeft;
int previousRight;
float currentDistance = 0;



void resetEncoders() {
    left.reset();
    right.reset();
}

void updateVector() {
    currentAngle = -1 * gyro.get_value();
    currentDistance = (float) ((left.get_value() - previousLeft) + (right.get_value() - previousRight)) / 2;
    if (currentAngle != previousAngle) {
        X_saved += (currentDistance * std::sin((((double) previousAngle) * PI) / 1800));
        Y_saved += (currentDistance * std::cos((((double) previousAngle) * PI) / 1800));
        //resetEncoders();
        currentDistance = 0;
        previousLeft = left.get_value();
        previousRight = right.get_value();
    }
    previousAngle = currentAngle;
    X_Pos = X_saved + (currentDistance * std::sin((((float) previousAngle) * PI) / 1800));
    Y_Pos = Y_saved + (currentDistance * std::cos((((float) previousAngle) * PI) / 1800));
}

void calculatePos(void *para) {
    while (true) {
        updateVector();
        pros::delay(10);
    }
}

