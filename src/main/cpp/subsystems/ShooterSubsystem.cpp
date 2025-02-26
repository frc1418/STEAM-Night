/**
 * Container file for the intake system
 * 
 * @author Edan Thomton
 * @date 2/24/24
 */

#ifndef __INTAKE_SUBSYSTEM__
#define __INTAKE_SUBSYSTEM__

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

    private:

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

};

#endif