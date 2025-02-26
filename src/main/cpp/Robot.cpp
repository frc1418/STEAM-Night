// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

/**
 * Main robot code
 * 
 * @author Edan Thomton
 * @author WPILib
 */

#include <frc/Joystick.h>
#include <frc/XboxController.h>
#include <frc/TimedRobot.h>

#include "subsystems/TankSubsystem.cpp"
#include "subsystems/ShooterSubsystem.cpp"

// if I don't have access to the joysticks or my own, xbox it is
// xbox = 0
// joystick = 1
#define JOYSTICK 1

using namespace std;
using namespace frc;
using namespace rev::spark;

class Robot : public TimedRobot {

    TankDrive base {SparkMaxConfig::IdleMode::kBrake};

    Shooter shooter;

    #if JOYSTICK
    Joystick joystick{0};
    #else
    XboxController xbox{0};
    #endif

    private:

        bool is_slow = false;
        /**
         * Toggles whether the robot is in "child" mode
         */
        void toggleSpeed() {
            if(is_slow) {
                // normal speed
                base.setArcadeMultipliers(1.00, 0.50);
                is_slow = false;
            } else {
                // child mode
                base.setArcadeMultipliers(0.25, 0.30);
                is_slow = true;
            }
        }

    public:

        /**
         * Robot initialization function
         */
        Robot() {

            toggleSpeed();

        }

        /**
         * Main function
         */
        void TeleopPeriodic() override {

            // driving stuff
            #if JOYSTICK

                base.arcade(
                    joystick.GetY(), -joystick.GetX()
                );

            #else

                base.arcade(
                    xbox.GetRightY(), -xbox.GetRightX()
                );

            #endif

            
            // funny input sequence so that children don't accidentally engage fast mode
            // (ignore the xbox controller one here it is boring)
            #if JOYSTICK
                const bool speedButtonPressed = 
                    joystick.GetRawButton(2) &&
                    joystick.GetRawButton(6) &&
                    joystick.GetRawButtonPressed(11);
            #else
                const bool speedButtonPressed = xbox.GetRightBumperPressed();
            #endif

            // speed mode
            if(speedButtonPressed) {
                toggleSpeed();
            }

            #if JOYSTICK

                // trigger button on my logitek joystick
                const bool outputButtonPressed = joystick.GetRawButton(1);

            #endif


            if(outputButtonPressed) {
                
                shooter.setOutput();

            } else {

                shooter.setIdle();

            }

        }
};

#ifndef RUNNING_FRC_TESTS
int main() {
    return StartRobot<Robot>();
}
#endif
