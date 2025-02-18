// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/Joystick.h>
#include <frc/XboxController.h>
#include <frc/TimedRobot.h>

#include <rev/config/SparkMaxConfig.h>
#include <rev/SparkMax.h>

#include <iostream>

// xbox = 0
// joystick = 1
#define JOYSTICK 1

using namespace std;
using namespace frc;
using namespace rev::spark;

class Robot : public TimedRobot {

    static const int 
        leftLID = 4,
        leftFID = 1,
        rightLID = 2,
        rightFID = 3;

    SparkMaxConfig leftLConfig;
    SparkMaxConfig leftFConfig;
    SparkMaxConfig rightLConfig;
    SparkMaxConfig rightFConfig;

    SparkMax  m_leftL{leftLID,  SparkMax::MotorType::kBrushed};
    SparkMax  m_leftF{leftFID,  SparkMax::MotorType::kBrushed};
    SparkMax m_rightL{rightLID, SparkMax::MotorType::kBrushed};
    SparkMax m_rightF{rightFID, SparkMax::MotorType::kBrushed};

    #if JOYSTICK
    Joystick joystick{0};
    #else
    XboxController xbox{0};
    #endif

    double speed_mul = 1.0;
    double rot_mul = 1.0;
    bool is_slow = false;

    private:
        void configure() {
            m_leftL.Configure(
                leftLConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kNoPersistParameters
            );
            m_leftF.Configure(
                leftFConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kNoPersistParameters
            );
            m_rightL.Configure(
                rightLConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kNoPersistParameters
            );
            m_rightF.Configure(
                rightFConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kNoPersistParameters
            );
        }

        void toggleSpeed() {
            if(is_slow) {
                // normal speed
                speed_mul = 1.00;
                rot_mul = 0.50;
                is_slow = false;
            } else {
                // child mode
                speed_mul = 0.25;
                rot_mul = 0.35;
                is_slow = true;
            }
        }

    public:
        Robot() {
            leftLConfig.SmartCurrentLimit(50).SetIdleMode(SparkMaxConfig::IdleMode::kBrake);
            rightLConfig.Apply(leftLConfig).Inverted(true);

            leftFConfig.Apply(leftLConfig).Follow(m_leftL);
            rightFConfig.Apply(rightLConfig).Follow(m_rightL);

            configure();
        }

        void TeleopPeriodic() override {

            // driving stuff

            #if JOYSTICK
                double speed = joystick.GetY() * speed_mul;
                double rot = joystick.GetX() * rot_mul;
            #else
                double speed = xbox.GetRightY() * speed_mul;
                double rot = xbox.GetRightX() * rot_mul;
            #endif

            double leftForward = speed - rot;
            double rightForward = speed + rot;

            m_leftL.Set(leftForward);
            m_rightL.Set(rightForward);

            // braking mode

            #if JOYSTICK
                const bool speedButtonPressed = 
                    joystick.GetRawButton(2) &&
                    joystick.GetRawButton(6) &&
                    joystick.GetRawButtonPressed(11);
            #else
                const bool speedButtonPressed = xbox.GetRightBumperPressed();
            #endif

            if(speedButtonPressed) {
                toggleSpeed();
            }
        }
};

#ifndef RUNNING_FRC_TESTS
int main() {
    return StartRobot<Robot>();
}
#endif
