/**
 * Container file for the tank drive base
 * 
 * @author Edan Thomton
 * @date 2/18/24
 */

/*
 * NOTE FOR 2/22 PROGRAMMERS:
 *
 * This file works as intended. Do not change.
 */

#ifndef __TANK_SUBSYSTEM__
#define __TANK_SUBSYSTEM__

#include <rev/config/SparkMaxConfig.h>
#include <rev/SparkMax.h>

using namespace rev::spark;

/**
 * Tank drive container class
 */
class TankDrive {

    // spark max configs, L = leader, F = follower
    SparkMaxConfig leftLConfig;
    SparkMaxConfig leftFConfig;
    SparkMaxConfig rightLConfig;
    SparkMaxConfig rightFConfig;

    // spark maxes (Leaders are the forward-most motors)
    // we are using CIMs so kBrushed it is
    SparkMax  leftL{4, SparkMax::MotorType::kBrushed};
    SparkMax  leftF{1, SparkMax::MotorType::kBrushed};
    SparkMax rightL{2, SparkMax::MotorType::kBrushed};
    SparkMax rightF{3, SparkMax::MotorType::kBrushed};

    // default multipliers
    double speedMultiplier    = 1.0;
    double rotationMultiplier = 1.0;

    public:
        TankDrive() {}

        /**
         * Constructor
         * 
         * @param brake_mode Tank drive braking mode, either Braked or Coasting
         */
        TankDrive(
            SparkMaxConfig::IdleMode brake_mode
        ) {
            // main sparkmax config
            leftLConfig
                .SmartCurrentLimit(50)
                .SetIdleMode(
                    brake_mode
                );
            // right side is inverted
            rightLConfig.Apply(leftLConfig).Inverted(true);

            // follower motors
            leftFConfig.Apply(leftLConfig).Follow(leftL);
            rightFConfig.Apply(rightLConfig).Follow(rightL);

            // configure the sparkmaxes
            leftL.Configure(
                leftLConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kNoPersistParameters
            );
            leftF.Configure(
                leftFConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kNoPersistParameters
            );
            rightL.Configure(
                rightLConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kNoPersistParameters
            );
            rightF.Configure(
                rightFConfig,
                SparkMax::ResetMode::kResetSafeParameters,
                SparkMax::PersistMode::kNoPersistParameters
            );
        }

        /**
         * Periodical update for tank-styled driving
         * 
         * @param left  left-side speed
         * @param right right-side speed
         */
        void tank(double left, double right) {
            leftL.Set(left * speedMultiplier);
            rightL.Set(right * speedMultiplier);
        }

        /**
         * Periodical update for arcade-styled driving
         * 
         * @param speed forward speed
         * @param rot   rotation speed
         */
        void arcade(double speed, double rot) {

            double fSpeed = speed * speedMultiplier;
            double fRot = rot * rotationMultiplier;

            double lforward = fSpeed + fRot;
            double rforward = fSpeed - fRot;

            leftL.Set(lforward);
            rightL.Set(rforward);
        }

        /**
         * Speed multiplier for tank-styled driving
         * 
         * @param multiplier speed multiplier
         */
        void setTankSpeedMultiplier(double multiplier) {
            speedMultiplier = multiplier;
        }

        /**
         * Speed multipliers for arcade-styled driving
         * 
         * @param speed_mul speed multiplier
         * @param rotation_mul rotation multiplier
         */
        void setArcadeMultipliers(double speed_mul, double rotation_mul) {
            speedMultiplier = speed_mul;
            rotationMultiplier = rotation_mul;
        }

};

#endif