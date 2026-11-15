/*
motor stall detection uses a current threshold of 0.5 A and a 3000 ms timeout
Those values need testing on the actual robot. 
A motor stall can also indicate a jam, not necessarily that the motor reached its hard stop.  
*/


void sensorCalibration()
{

    // CALIBRATE TILT TO 0 DEGREES

    tilt.spin(reverse, 10, percent);

    timer1.clear();
    int tiltStall = 0;

    while (timer1.time(msec) < 3000 && tiltStall < 5)
    {
        double oldPosition = tilt.position(degrees);

        wait(100, msec);

        double newPosition = tilt.position(degrees);
        double motorCurrent = tilt.current(amp);

        double change = newPosition - oldPosition;

        if (change < 1 && change > -1 &&
            motorCurrent > 0.5)
        {
            tiltStall++;
        }
        else
        {
            tiltStall = 0;
        }
    }

    tilt.stop(brake);

    if (tiltStall >= 5)
    {
        tilt.setPosition(0, degrees);
    }
    else
    {
        Brain.Screen.print("Tilt calibration failed");
        return;
    }


    // CALIBRATE SPIN TO -140 DEGREES

    spin.spin(reverse, 10, percent);

    timer1.clear();
    int spinStall = 0;

    while (timer1.time(msec) < 3000 && spinStall < 5)
    {
        double oldPosition = spin.position(degrees);

        wait(100, msec);

        double newPosition = spin.position(degrees);
        double motorCurrent = spin.current(amp);

        double change = newPosition - oldPosition;

        if (change < 1 && change > -1 &&
            motorCurrent > 0.5)
        {
            spinStall++;
        }
        else
        {
            spinStall = 0;
        }
    }

    spin.stop(brake);

    if (spinStall >= 5)
    {
        spin.setPosition(-140, degrees);
    }
    else
    {
        Brain.Screen.print("Spin calibration failed");
        return;
    }


    // CALIBRATE INERTIAL SENSOR

    BrainInertial.resetRotation();
    BrainInertial.calibrate();

    while (BrainInertial.isCalibrating())
    {
    }

    Brain.Screen.print("Calibration complete");
}




// SAFEER: Initialize teleop mode
bool initTeleopMode()
{
    // Check that controller is connected
    bool controllerReady = Controller.installed();

    // Check that required motors are online
    bool motorsReady = leftMotor.installed() && rightMotor.installed() && spinMotor.installed() && tiltMotor.installed();

    // Check that required sensors are online
    bool sensorsReady = distanceSensor.installed() && BrainInertial.installed();

    // Only allow teleop if everything is ready
    if (controllerReady && motorsReady && sensorsReady)
    {
        Brain.Screen.clearScreen();
        Brain.Screen.print("Teleop Ready!");

        return true;
    }
    else
    {
        // Stop all motors
        leftMotor.stop();
        rightMotor.stop();
        spinMotor.stop();
        tiltMotor.stop();

        // Display error message
        Brain.Screen.clearScreen();
        Brain.Screen.print("SYSTEM ERROR!");

        if (!controllerReady)
        {
            Brain.Screen.newLine();
            Brain.Screen.print("Controller offline");
        }

        if (!motorsReady)
        {
            Brain.Screen.newLine();
            Brain.Screen.print("Motor offline");
        }

        if (!sensorsReady)
        {
            Brain.Screen.newLine();
            Brain.Screen.print("Sensor offline");
        }

        return false;
    }
}


// SAFEER: Get controller controls
void getControls(int &forwardInput, int &turnInput,
                 bool &scanButton, bool &stopButton)
{
    // Read joystick positions
    forwardInput = Controller.AxisD.position();
    turnInput = Controller.AxisA.position();

    // Read button inputs
    scanButton = Controller.ButtonEUp.pressing();
    stopButton = Controller.ButtonL3.pressing();
}


// Main program
int main()
{
    int forwardInput = 0;
    int turnInput = 0;

    bool scanButton = false;
    bool stopButton = false;
    
	// to call the function
    bool ready = initTeleopMode();


    	// to call the function
	getControls(forwardInput, turnInput, scanButton, stopButton);
	
   
    return 0;
}
