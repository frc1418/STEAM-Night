/**
 * Container file for the intake system
 * 
 * @author Edan Thomton
 * @date 2/24/24
 */

#ifndef __INTAKE_SUBSYSTEM__
#define __INTAKE_SUBSYSTEM__

#include <ctime>

#include <rev/config/SparkMaxConfig.h>
#include <rev/SparkMax.h>

using namespace rev::spark;
 
/**
 * Shooter subsystem container
 */
class Shooter {

    SparkMaxConfig leftConfig;
    SparkMaxConfig rightConfig;

    SparkMax left{5, SparkMax::MotorType::kBrushless};
    SparkMax right{6, SparkMax::MotorType::kBrushless};

    int lastShot = 99999999999;

    public:
        /**
         * Default constructor and initializor
         */
        Shooter(){

            leftConfig.SmartCurrentLimit(50)
                .SetIdleMode(
                    SparkMaxConfig::IdleMode::kBrake
                );
            rightConfig.Apply(leftConfig);

            left.Configure(
                leftConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kPersistParameters
            );

            right.Configure(
                rightConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kPersistParameters
            );

        }

        /**
         * Set motors to intake
         */
        void setIntake() {

            left.Set(0.2);
            right.Set(-0.2);

        }
        /**
         * Set motors to output
         */
        void setOutput() {

            left.Set(-0.4);
            right.Set(0.4);

        }
        /**
         * Set motors to idle
         */
        void setIdle() {
            left.Set(0.0);
            right.Set(0.0);
        }

        /**
         * Periodic ticker function that allows for
         * a specified rate of fire.
         * This gets called every 20ms, so
         * `fireTime = 10 * 20ms = 200ms = 0.2s`
         */
        void periodic() {
            const int fireTime = 10;

            if(lastShot < fireTime) {
                setOutput();
            } else {
                setIdle();
            }

            lastShot++;
        }

        /**
         * Shooting method, resets cooldown
         * `periodic()` gets called every 20ms,
         * so `cooldown = 60 * 20ms = 1200ms = 1.2s`
         */
        void shoot() {
            const int cooldown = 60;

            if(lastShot > cooldown) {
                lastShot = 0;
            }
        }

};

#endif