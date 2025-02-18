/**
 * Container file for the tank drive base
 */

#include <rev/config/SparkMaxConfig.h>
#include <rev/SparkMax.h>

#include <iostream>

using namespace std;
using namespace frc;
using namespace rev::spark;

enum TankDriveMode {
    coasting = 0U,
    braked   = 1U
};

class TankDrive {

    SparkMaxConfig leftLConfig;
    SparkMaxConfig leftFConfig;
    SparkMaxConfig rightLConfig;
    SparkMaxConfig rightFConfig;

    SparkMax  leftL{4,  SparkMax::MotorType::kBrushed};
    SparkMax  leftF{1,  SparkMax::MotorType::kBrushed};
    SparkMax rightL{2, SparkMax::MotorType::kBrushed};
    SparkMax rightF{3, SparkMax::MotorType::kBrushed};

    public:
        TankDrive(
            TankDriveMode brake_mode
        ) {
            leftLConfig
                .SmartCurrentLimit(50)
                .SetIdleMode(
                    (SparkMaxConfig::IdleMode) brake_mode // me when custom enum go brr (it works enums are just integer values)
                );
            rightLConfig.Apply(leftLConfig).Inverted(true);

            leftFConfig.Apply(leftLConfig).Follow(leftL);
            rightFConfig.Apply(rightLConfig).Follow(rightL);

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

        void updateTank(double left, double right) {
            leftL.Set(left);
            rightL.Set(right);
        }
        void updateArcade(double speed, double rot) {
            double lforward = speed + rot;
            double rforward = speed - rot;

            leftL.Set(lforward);
            rightL.Set(rforward);
        }

};