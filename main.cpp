/********************************/
/*      Proteus Test Code       */
/*     OSU FEH Spring 2020      */
/*        Josh Mulyadi        */
/*    02/03/20  Version 3.0.1     */
/********************************/

// AM 02/03/20

/* Include preprocessor directives */
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHRPS.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
/* Define colors for parts of menus */
#define MENU_C WHITE
#define TEXT_C GOLD
#define SELT_C RED
#define SHOW_C BLUE
#define HI_C GREEN

/* Define menu number values */
#define MN_MENU 0
#define DC_MENU 1
#define SV_MENU 2
#define DI_MENU 3
#define AI_MENU 4
#define AC_MENU 5
#define TO_MENU 6
#define DO_MENU 7
#define RP_MENU 8

/* Define time for beep */
#define beep_t 10 // int milliseconds

/* Global variable to keep track of being initialized to RPS */
int RPS_init = 0;
AnalogInputPin sensor(FEHIO::P1_0);
DigitalEncoder left_encoder(FEHIO::P0_1);
DigitalEncoder right_encoder(FEHIO::P0_2);
FEHMotor right_motor(FEHMotor::Motor2,9.0);
//goes backwords
FEHMotor left_motor(FEHMotor::Motor0,9.0);
//positive reel in negative reel out
FEHMotor reel(FEHMotor::Motor1,9.0);
DigitalEncoder reel_Encoder(FEHIO::P0_0);
DigitalInputPin fr(FEHIO::P2_0);
DigitalInputPin fl(FEHIO::P2_1);
DigitalInputPin br(FEHIO::P2_2);
DigitalInputPin bl(FEHIO::P2_3);
AnalogInputPin middlePins(FEHIO::P3_0);
double lightSense(){
    return sensor.Value();
}
void showCaseLight(){
    double time = TimeNow();
    while(true){
        LCD.WriteLine(sensor.Value());
    }
}
void pinWheel(){
    FEHServo pinWheel(FEHServo::Servo0);
    pinWheel.SetMin(998);
    pinWheel.SetMax(1852);
    while(true){
    double x = 180/3.3;
    pinWheel.SetDegree(lightSense()*x);
    }
}
enum LineStates{
    MIDDLE,
    RIGHT,
    LEFT
};
/*void line(){
    int state=MIDDLE;
    while(true){
        switch(state){
            case MIDDLE:
                Sleep(10);
                LCD.Clear();
                LCD.Write("Mid");
                LCD.WriteLine(left.Value());
    
                LCD.WriteLine(mid.Value());
    
                LCD.WriteLine(right.Value());
                right_motor.SetPercent(25);
                left_motor.SetPercent(25);
                if(right.Value()>2){
                    state=RIGHT;
                }
                else if(left.Value()>2){
                    state=LEFT;
                }
                break;
            case RIGHT:
                Sleep(10);
                LCD.Clear();
                LCD.Write("Right");
                LCD.WriteLine(left.Value());
                LCD.WriteLine(mid.Value());
                LCD.WriteLine(right.Value());
                right_motor.SetPercent(5);
                left_motor.SetPercent(30);
                if(mid.Value()>2){
                    state=MIDDLE;
                }
                break;
            case LEFT:
                Sleep(10);
                LCD.Clear();
                LCD.Write("Left");
                LCD.WriteLine(left.Value());
                LCD.WriteLine(mid.Value());
                LCD.WriteLine(right.Value());
                right_motor.SetPercent(30);
                left_motor.SetPercent(5);
                if(mid.Value()>2){
                    state=MIDDLE;
                }
                break;
        }
    }
}*/
void turn(int degree){
    //turn right
    int rightPercent = -35;
    int leftPercent = -35;
    //turns left
    if(degree < 0){
        rightPercent = 35;
        leftPercent = 35;
        degree *= -1;
    }
    double time = 1.62 * degree/100.0;
    right_motor.SetPercent(rightPercent);
    left_motor.SetPercent(leftPercent);
    Sleep(time);
    right_motor.Stop();
    left_motor.Stop();
}
void forward(float inch, int percent){
    //32
    //20
    //25
    right_motor.SetPercent(percent);
    left_motor.SetPercent((percent+1) * -1);
    double distance = inch * .224;
    Sleep(distance);
    LCD.Write(RPS.X());
    LCD.Write(RPS.Y());
    LCD.Write(RPS.Heading());
    right_motor.Stop();
    left_motor.Stop();
}
void bumpForward(){
    right_motor.SetPercent(25);
    left_motor.SetPercent(-26);
    while(fr.Value() || fl.Value())
    {
        Sleep(.5);
        if(!fr.Value()){
            left_motor.SetPercent(-30);
        }
        if(!fl.Value()){
            right_motor.SetPercent(30);
        }
    }
    right_motor.Stop();
    left_motor.Stop();
    
}
void bumpBack(){
    right_motor.SetPercent(-25);
    left_motor.SetPercent(26);
    while(br.Value() && bl.Value());
    do{
         if(!br.Value()){
            
             right_motor.SetPercent(-40);
        }
        else if(!bl.Value()){
            
            left_motor.SetPercent(40);
        }
    }while(br.Value() || bl.Value());

    right_motor.Stop();
    left_motor.Stop();
    right_motor.SetPercent(25);
    left_motor.SetPercent(-26);
    Sleep(.5);
    right_motor.Stop();
    left_motor.Stop();
    Sleep(.5);
}

void check1(){
    while(lightSense()>1);
    forward(2,25);
    turn(53);
    forward(30, 29);
    turn(-90);
    forward(13, 25);
    turn(90);
    forward(15, 25);
}
void back(){
    right_motor.SetPercent(-25);
    left_motor.SetPercent(25);
    Sleep(1.0);
    right_motor.Stop();
    left_motor.Stop();
    turn(-180);
    forward(11, 25);
    turn(-85);
    bumpForward();
    turn(85);
    forward(25, 25);
}

void check2(){
    float color=3.3;
    while(lightSense()>1);
    forward(2,25);
    turn(51);
    forward(30, 29);
    turn(-83);
    bumpBack();
    forward(18.89, 25);
    turn(86);
    right_motor.SetPercent(25);
    left_motor.SetPercent(-26);
    float startTime=TimeNow();
    while(true){
        if(lightSense()<color){
            color=lightSense();
        }
        LCD.Clear();
        LCD.WriteLine(color);
        if(TimeNow()-startTime>3.7){
            break;
        }
    }
    right_motor.Stop();
    left_motor.Stop();
    if(color>.5){
        LCD.WriteAt("BLUE", 100, 100);
    }else if(color<.5){
        LCD.WriteAt("RED", 100, 100);
    }
    forward(10,-25);
    turn(80);
    bumpBack();
    if(color>.5){
        forward(11,25);
        
    }else{
        forward(15,25);
    }
    turn(-90);
    if(color<0.5){
    forward(10.5,25);
}else{
    forward(10.5,25);
}

    back();
}

void check3(){
    while(lightSense()>1);
    int lever = RPS.GetCorrectLever();
    LCD.WriteLine(lever);
    forward(17,25);
    right_motor.SetPercent(40);
    left_motor.SetPercent(40);
    Sleep(.75);
    right_motor.Stop();
    left_motor.Stop();
    bumpForward();
    int turn = 40;
    switch(lever){
        case 0:
            forward(5,-25);
            break;
        case 1:
            forward(2,-25);
            break;
        case 2:
            turn = 36;
            break;
    }
    right_motor.SetPercent(turn);
    left_motor.SetPercent(turn);
    Sleep(1.54);
    right_motor.Stop();
    left_motor.Stop();
    reel.SetPercent(95);
    Sleep(.7);
    reel.Stop();
    //go back
    forward(4,-25);
    Sleep(1.0);
    forward(3.4,25);
    Sleep(5.0);
    right_motor.SetPercent(-30);
    left_motor.SetPercent(30);
    reel.SetPercent(-69);
    Sleep(.420);
    reel.Stop();
    reel.SetPercent(69);
    Sleep(.420);
    reel.Stop();
    forward(5,-25);
    reel.SetPercent(-69);
    Sleep(1.5);
    reel.Stop();
}
void check4(){
    while(lightSense()>1){
        LCD.Write(lightSense());
    }
    LCD.Clear();
    forward(30, 32.5);
    turn(-90);
    bumpBack();
    forward(16, 25);
    reel.SetPercent(45);
    Sleep(1.6);
    reel.Stop();
    turn(93);
    forward(7, 25);
    turn(93);
    forward(5.2, 25);
    reel.SetPercent(-50);
    Sleep(.35);
    right_motor.SetPercent(15);
    left_motor.SetPercent(-15);
    Sleep(.5);
    reel.Stop();
    turn(-30);
    turn(30);
    forward(4,-25);
    turn(-70);
    forward(5,25);
    turn(120);
    reel.SetPercent(-50);
    Sleep(.8);
    reel.Stop();
}
void check5(){
    while(lightSense()>1);
    right_motor.SetPercent(40);
    left_motor.SetPercent(-40);
    while(fr.Value());
    right_motor.Stop();
    left_motor.Stop();
    right_motor.SetPercent(40);
    left_motor.SetPercent(40);
    Sleep(.95);
    right_motor.Stop();
    left_motor.Stop();
    forward(14, 35);
    right_motor.SetPercent(40);
    left_motor.SetPercent(-20);
    Sleep(.5);
    right_motor.Stop();
    left_motor.Stop();
    forward(10.8, 35);
    right_motor.SetPercent(40);
    left_motor.SetPercent(40);
    Sleep(1.54);
    right_motor.Stop();
    left_motor.Stop();
    bumpBack();
    forward(18, 25);
    right_motor.SetPercent(40);
    left_motor.SetPercent(40);
    Sleep(1.9);//special
    right_motor.Stop();
    left_motor.Stop();
    forward(6,25);
    reel.SetPercent(45);
    Sleep(1.5);
    reel.Stop();
    forward(7, -30);
    reel.SetPercent(-50);
    Sleep(1.8);
    reel.Stop();
    forward(4, 30);
    right_motor.SetPercent(-40);
    left_motor.SetPercent(-40);
    Sleep(1.4);//different
    right_motor.Stop();
    left_motor.Stop();
    bumpBack();
    forward(2,25);
    right_motor.SetPercent(40);
    left_motor.SetPercent(40);
    Sleep(1.65);
    right_motor.Stop();
    left_motor.Stop();
    forward(20,50);
}
void turnLeft(){
    right_motor.SetPercent(40);
    left_motor.SetPercent(40);
    Sleep(1.54);
    right_motor.Stop();
    left_motor.Stop();
}
void turnRight(){
    right_motor.SetPercent(-40);
    left_motor.SetPercent(-40);
    Sleep(1.54);
    right_motor.Stop();
    left_motor.Stop();
}
void check51(){
    forward(8,35);
    turn(50);
    forward(2,35);
    reel.SetPercent(45);
    Sleep(1.5);
    reel.Stop();
    forward(7, -30);
    reel.SetPercent(-50);
    Sleep(1.8);
    reel.Stop();
}
void reelIn(){
    reel.SetPercent(-50);
    Sleep(.2);
    reel.Stop();
}
void reelOut(){
    reel.SetPercent(50);
    Sleep(.2);
    reel.Stop();
}

// Number of points of interest (i.e. A, B, C, D)
#define NUM_POINTS_OF_INTEREST 5

// RPS Delay time
#define RPS_WAIT_TIME_IN_SEC 0.35

// Shaft encoding counts for CrayolaBots
#define COUNTS_PER_INCH 40.5
#define COUNTS_PER_DEGREE 2.48

/* Defines for how long each pulse should be and at what motor power. 
These value will normally be small, but you should play around with the values to find what works best */
#define PULSE_TIME 0.125
#define PULSE_POWER 30

// Define for the motor power while driving (not pulsing)
#define POWER 25

#define HEADING_TOLERANCE 2

/* Direction along axis which robot is traveling
Examples:
	- if robot is traveling to the upper level, that is a PLUS as the y-coordinate is increasing
	- if robot is traveling to the lower level, that is a MINUS as the y-coordinate is decreasing
*/
#define PLUS 0
#define MINUS 1

// Declarations for encoders & motors


void set_points_of_interest()
{
    // Declare variables
    float touch_x, touch_y;
    char points[NUM_POINTS_OF_INTEREST] = {'A', 'B', 'C', 'D', 'E'};

    // Open SD file for writing
    FEHFile *fptr = SD.FOpen("RPS_POIs.txt", "w");\

    Sleep(100);
    LCD.Clear();

    // Wait for touchscreen to be pressed and released
    LCD.WriteLine("Press Screen to Record");
    while (!LCD.Touch(&touch_x, &touch_y));
    while (LCD.Touch(&touch_x, &touch_y));

    LCD.ClearBuffer();

    // Clear screen
    Sleep(100); // wait for 100ms to avoid updating the screen too quickly
    LCD.Clear();

    // Write initial screen info
    LCD.WriteRC("X Position:", 11, 0);
    LCD.WriteRC("Y Position:", 12, 0);
    LCD.WriteRC("   Heading:", 13, 0);

    // Step through each path point to record position and heading
    for (int n = 0; n < NUM_POINTS_OF_INTEREST; n++)
    {
        // Write point letter
        LCD.WriteRC("Touch to set point ", 9, 0);
        LCD.WriteRC(points[n], 9, 20);

        // Wait for touchscreen to be pressed and display RPS data
        while (!LCD.Touch(&touch_x, &touch_y))
        {
            LCD.WriteRC(RPS.X(), 11, 12);       // update the x coordinate
            LCD.WriteRC(RPS.Y(), 12, 12);       // update the y coordinate
            LCD.WriteRC(RPS.Heading(), 13, 12); // update the heading

            Sleep(100); // wait for 100ms to avoid updating the screen too quickly
        }
        while (LCD.Touch(&touch_x, &touch_y));
        LCD.ClearBuffer();

        // Print RPS data for this path point to file
        SD.FPrintf(fptr, "%f %f\n", RPS.X(), RPS.Y());
    }

    // Close SD file
    SD.FClose(fptr);
    LCD.Clear();
}

/*
 * Pulse forward a short distance using time
 */
void pulse_forward(int percent, float seconds)
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Pulse counterclockwise a short distance using time
 */
void pulse_counterclockwise(int percent, float seconds)
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Move forward using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void move_forward(int percent, int counts) // using encoders
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Turn counterclockwise using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void turn_counterclockwise(int percent, int counts)
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Use RPS to move to the desired x_coordinate based on the orientation of the QR code
 */
void check_x(float x_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code
    int power = PULSE_POWER;
    if (orientation == MINUS)
    {
        power = -PULSE_POWER;
    }

    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while (RPS.X()>0 && (RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1))
    {
        if (RPS.X() > x_coordinate + 1)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-power, PULSE_TIME);
        }
        else if (RPS.X() < x_coordinate - 1)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(power, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
    while(RPS.X()==-2.0&&middlePins.Value()>2){
        right_motor.SetPercent(power);
        left_motor.SetPercent(power);
    }
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Use RPS to move to the desired y_coordinate based on the orientation of the QR code
 */
void check_y(float y_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code
    int power = PULSE_POWER;
    if (orientation == MINUS)
    {
        power = -PULSE_POWER;
    }

    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while (RPS.Y()>0 && (RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1))
    {
        if (RPS.Y() > y_coordinate + 1)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-power, PULSE_TIME);
        }
        else if (RPS.Y() < y_coordinate - 1)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(power, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
}
float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y, E_x, E_y;
// float A_x = 31.0;
// float A_y = 44.598;
// float B_x = 13.899;
// float B_y= 43.40;
// float C_x = 11.5;
// float C_y = 61.50;
// float D_x = 16.399;
// float D_y = 55.0;
// float E_x = 21.00;
// float E_y = 15.5;
/*
 * Use RPS to move to the desired heading
 */
void check_heading(float heading)
{
    // You will need to fill out this one yourself and take into account
    // checking for proper RPS data and the edge conditions
    //(when you want the robot to go to 0 degrees or close to 0 degrees)

    /*
        SUGGESTED ALGORITHM:
        1. Check the current orientation of the QR code and the desired orientation of the QR code
        2. Check if the robot is within the desired threshold for the heading based on the orientation
        3. Pulse in the correct direction based on the orientation
    */
float Start = TimeNow();
   while(RPS.Heading()>0&&(heading<RPS.Heading()-2||heading>RPS.Heading()+2)){
       
       if(TimeNow()-Start<5.0){
            if(heading<RPS.Heading()-1){
                if((heading>=354||heading<=2 )&&RPS.Heading()>180){
                     pulse_counterclockwise(PULSE_POWER,PULSE_TIME);

                }else{
                  pulse_counterclockwise(-PULSE_POWER,PULSE_TIME);
                 }
            
            }
             if(heading>RPS.Heading()+1){
                if((heading>=354||heading<=2 )&&RPS.Heading()<180){
                    pulse_counterclockwise(-PULSE_POWER,PULSE_TIME);
                }
                else{
                    pulse_counterclockwise(PULSE_POWER,PULSE_TIME);}
            }
            if(heading > RPS.Heading()-2 && heading<RPS.Heading()+2){
                break;
            }
       } else{
            if(heading<RPS.Heading()-1){
                if((heading>=354||heading<=2 )&&RPS.Heading()>180){
                     pulse_counterclockwise(PULSE_POWER,PULSE_TIME/2);

                }else{
                  pulse_counterclockwise(-PULSE_POWER,PULSE_TIME/2);
                 }
            
            }
             if(heading>RPS.Heading()+1){
                    if((heading>=355||heading<=2 )&&RPS.Heading()>180){
                    pulse_counterclockwise(-PULSE_POWER,PULSE_TIME/2);
                }
                else{
                    pulse_counterclockwise(PULSE_POWER,PULSE_TIME/2);}
            }
            if(heading > RPS.Heading()-2 && heading<RPS.Heading()+2){
                break;
            }
            Start=TimeNow();
       }

        Sleep(RPS_WAIT_TIME_IN_SEC);
   }
}
void upRamp(){
    forward(10, 35);
    right_motor.SetPercent(40);
    left_motor.SetPercent(-30);
    Sleep(.8);
    right_motor.Stop();
    left_motor.Stop();
    forward(14, 35);
    Sleep(0.3);
    check_heading(1);
    check_y(A_y+1, PLUS);
}
void luggage(){
    turn(-120);
     Sleep(0.3);
    check_heading(88);
    forward(10.3,35);
     Sleep(0.3);
    check_x(B_x-4, MINUS);
    turn(-105); //-105
     Sleep(0.3);
    check_heading(177);
     Sleep(0.3);
    check_y(B_y,MINUS);
    reel.SetPercent(-55);
    Sleep(1.2);
    reel.Stop();
    forward(3,-35);
    reel.SetPercent(55);
    Sleep(1.5);
    reel.Stop();
}
void checkLight(){
    // bumpBack();
    // forward(C_x/3,35);
    // turn(-110);
    // check_heading(0);
    // forward(C_y-B_y,35);
    // check_y(C_y, PLUS);
    //     float startTime=TimeNow();
    //     float color =3.3;
    // while(true){
    //     if(lightSense()<color){
    //         color=lightSense();
    //     }
    //     LCD.Clear();
    //     LCD.WriteLine(color);
    //     if(TimeNow()-startTime>3.7){
    //         break;
    //     }
    // }
    // right_motor.Stop();
    // left_motor.Stop();
    // if(color>.5){
    //     LCD.WriteAt("BLUE", 100, 100);
    // }else if(color<.5){
    //     LCD.WriteAt("RED", 100, 100);
    // }
    
    }
void lever(){
    forward(5.5,35);
    turn(-80);
     Sleep(0.3);
    check_x(E_x,MINUS);
    reel.SetPercent(-95);
    Sleep(.7);
    reel.Stop();
    //go back
    forward(3,-25);
    Sleep(1.0);
    forward(3,25);
    reel.SetPercent(69);
    Sleep(.23);
    reel.Stop();
    reel.SetPercent(-69);
    Sleep(.420);
    reel.Stop();
    reel.SetPercent(50);
    right_motor.SetPercent(-40);
    left_motor.SetPercent(40);
    Sleep(.5);
    right_motor.Stop();
    left_motor.Stop();
    Sleep(1.6);
    reel.Stop();
    }

int main(void)
{
    LCD.Write(Battery.Voltage());
    Sleep(2.0);
    int x;
    int y;
    LCD.Clear();
    
    
    LCD.WriteLine("Press Screen To Start Reel");
        while(!LCD.Touch(&x,&y));
        while(LCD.Touch(&x,&y));
    LCD.WriteLine("Reel In");
    Sleep(.2);
    reel.SetPercent(25);
    LCD.WriteLine("Press Screen To Stop Reel");
        while(!LCD.Touch(&x,&y));
        while(LCD.Touch(&x,&y));
    reel.Stop();
    RPS.InitializeTouchMenu();
    set_points_of_interest();
     // COMPLETE CODE HERE TO READ SD CARD FOR LOGGED X AND Y DATA POINTS
    FEHFile *fptr = SD.FOpen("RPS_POIs.txt", "r");
    SD.FScanf(fptr, "%f%f", &A_x, &A_y);
    SD.FScanf(fptr, "%f%f", &B_x, &B_y);
    SD.FScanf(fptr, "%f%f", &C_x, &C_y);
    SD.FScanf(fptr, "%f%f", &D_x, &D_y);
    SD.FScanf(fptr, "%f%f", &E_x, &E_y);
    SD.FClose(fptr);
    LCD.Write("X val : ");
    LCD.WriteLine(A_x);
    LCD.Write("Y Val: ");
    LCD.WriteLine(A_y);
    LCD.Write("X val : ");
    LCD.WriteLine(B_x);
    LCD.Write("Y val : ");
    LCD.WriteLine(B_y);
    LCD.Write("X val : ");
    LCD.WriteLine(C_x);
    LCD.Write("Y val : ");
    LCD.WriteLine(C_y);
    LCD.Write("X val : ");
    LCD.WriteLine(D_x);
    LCD.Write("Y val : ");
    LCD.WriteLine(D_y);
    LCD.Write("X val : ");
    LCD.WriteLine(E_x);
    LCD.Write("Y val : ");
    LCD.Write(E_y);
    //WRITE CODE HERE TO SET THE HEADING DEGREES AND COUNTS VALUES
    LCD.Clear();
    //RPS.InitializeTouchMenu();
    
    LCD.WriteLine("Press Screen To Start Run");
    while(!LCD.Touch(&x,&y));
    while(LCD.Touch(&x,&y));
    LCD.Clear();
    while(lightSense()>1);
    right_motor.SetPercent(40);
    left_motor.SetPercent(-40);
    while(fr.Value());
    right_motor.Stop();
    left_motor.Stop();
    turn(-63);
    Sleep(0.3);
    check_heading(357);
    upRamp();
    luggage();
    turn(90);
    Sleep(0.3);
    check_x(C_x-1.4,MINUS);
    turn(90);
    Sleep(0.3);
    check_heading(1);
    right_motor.SetPercent(35);
    left_motor.SetPercent(-35);
    double time = TimeNow();
    double time2 = TimeNow();
    float color = 3.3;
    while(true){
        if(TimeNow()-time>0.2){
           time=TimeNow();
            if(RPS.Y()>C_y){
    //important
                break;
            }
        }
  
        if(lightSense()<color){
            color=lightSense();
        }
        LCD.Clear();
        LCD.WriteLine(color);
        if(TimeNow()-time2>3.7){
            break;
        }
    }
    right_motor.Stop();
    left_motor.Stop();
    //New Code
    // check_y(C_y-.24, PLUS);//-.24
    // double time3 = TimeNow();
    // double time4 = TimeNow();
    // int per = 25;

    // while(true){
    //     if(TimeNow()-time3>=1.0){
    //         per *=-1; 
    //         time3=TimeNow();
    //     }
    //     right_motor.SetPercent(per);
    //     left_motor.SetPercent(per);
        
    //     if(lightSense()<color){
    //         color=lightSense();
    //         LCD.Clear();
    //         LCD.WriteLine(color);
    //     }
    //     if(TimeNow()-time4>=2.0){
    //         break;
    //     }
    //     Sleep(.15);
    //     right_motor.Stop();
    //     left_motor.Stop();
    // }

    // right_motor.Stop();
    // left_motor.Stop();
    // check_heading(0);
    //end of new code
    forward(4.5,-35);
    Sleep(.3);
    check_y(D_y+1.5,PLUS);//from 15.15, from B_y+14.98
    
    turn(75);
    Sleep(0.3);
    check_heading(269);
     if(color>.45){
        LCD.WriteAt("BLUE", 100, 100);
    }else if(color<.45){
        LCD.WriteAt("RED", 100, 100);
    }
//if it lines up with stamp task
reel.SetPercent(-55);
Sleep(1.7);
reel.Stop();
forward(6,35);
reel.SetPercent(65);
Sleep(0.6);
right_motor.SetPercent(25);
left_motor.SetPercent(-25);
Sleep(.5);
reel.Stop();
reel.SetPercent(55);
Sleep(.35);
reel.Stop();
left_motor.Stop();
right_motor.Stop();
right_motor.SetPercent(60);
left_motor.SetPercent(20);
Sleep(.82);//0.7_>0.82
left_motor.Stop();
right_motor.Stop();
right_motor.SetPercent(-20);
left_motor.SetPercent(-50);
Sleep(.65);///0.5->0.65
left_motor.Stop();
right_motor.Stop();
forward(2,-25);//from 1-2
reel.SetPercent(55);
Sleep(.5);
reel.Stop();
check_heading(270);


//press button
    if(color>.6){
        forward(.7,-35);//changed from 0.65 forward
        turn(-92);
         Sleep(0.3);
        check_heading(0);
    }else{
        forward(.8,35);//changed from 2 forward
        turn(-45);
        forward(2,35);
        turn(-50);
         Sleep(0.3);
        check_heading(0);
        
    }
    forward(1,-55);
    reel.SetPercent(-55);
    Sleep(1.7);
    reel.Stop();
    forward(5.5, 35);//added 1(just in case doesnt reach)
    // if(color<0.6){//changed to 0.6
    //     right_motor.SetPercent(20);
    //     left_motor.SetPercent(-10);
    //     Sleep(1.5);
    //     right_motor.Stop();
    //     left_motor.Stop();
    // }
    reel.SetPercent(55);
    right_motor.SetPercent(-10);
    left_motor.SetPercent(10);
    Sleep(2.0);
    reel.Stop();
    right_motor.Stop();
    left_motor.Stop();
    if(color<.45){
        turn(-110);
       forward(2,35);
        turn(-30);
    }else{
        forward(1,35);//ADDED
        turn(-135);
    }
    right_motor.SetPercent(40);
    left_motor.SetPercent(-40);
    while(fr.Value());
    right_motor.Stop();
    left_motor.Stop();
    turn(-30);
    forward(1.5,35);
    turn(-40);
     Sleep(0.3);
    check_heading(175);
     Sleep(0.3);
    check_y(B_y,MINUS);
    forward(6.5,40);
    Sleep(0.3);
    if(RPS.Y()<= -0.80){
        forward(2,-35);
    }
    Sleep(0.3);
    check_heading(180);
    Sleep(0.3);
    check_y(E_y+7.3,MINUS);
     Sleep(0.3);
    check_heading(207);
     Sleep(0.3);
    check_y(E_y+3,MINUS);
    reel.SetPercent(-55);
    Sleep(1.30);//from 1.48 then 1.35
    
    forward(2.5,-35);
    forward(2.6,35);
    reel.Stop();//new
    //Sleep(5.0);// new
    reel.SetPercent(70);
    Sleep(.45);
    right_motor.SetPercent(-10);
    left_motor.SetPercent(10);
    Sleep(1.0);
    reel.Stop();
    right_motor.Stop();
    left_motor.Stop();
    reel.SetPercent(-55);
    Sleep(.3);
    forward(2,-35);
    reel.SetPercent(55);
    Sleep(2.0);
    reel.Stop();
    Sleep(0.3);
    check_y(E_y+4.0, MINUS);
    turn(-70);
     Sleep(0.3);
    check_heading(270);
     Sleep(0.3);
     forward(3,55);
    check_x(B_x+3.3,PLUS);
    //turn(60);
    Sleep(0.3);
    turn(40);
    check_heading(225);
    right_motor.SetPercent(80);
    left_motor.SetPercent(-80);
    Sleep(6.0);
    right_motor.Stop();
    left_motor.Stop();
    return 0;
}
/*int main(void)
{
    float touch_x, touch_y;

    float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y;
    float A_heading, B_heading, C_heading, D_heading;
    int B_C_counts, C_D_counts, turn_90_counts, turn_180_counts;

    RPS.InitializeTouchMenu();

    set_points_of_interest();

    LCD.Clear();
    LCD.WriteLine("Press Screen To Start Run");
    while (!LCD.Touch(&touch_x, &touch_y));
    while (LCD.Touch(&touch_x, &touch_y));

    // COMPLETE CODE HERE TO READ SD CARD FOR LOGGED X AND Y DATA POINTS
    FEHFile *fptr = SD.FOpen("RPS_POIs.txt", "r");
    SD.FScanf(fptr, "%f%f", &A_x, &A_y);
    SD.FScanf(fptr, "%f%f", &B_x, &B_y);
    SD.FScanf(fptr, "%f%f", &C_x, &C_y);
    SD.FScanf(fptr, "%f%f", &D_x, &D_y);
    SD.FClose(fptr);

    // WRITE CODE HERE TO SET THE HEADING DEGREES AND COUNTS VALUES
    A_heading = 180;
    B_heading = 270;
    C_heading = 90;
    D_heading = 0;

    B_C_counts = 17*COUNTS_PER_INCH;
    C_D_counts = 17*COUNTS_PER_INCH;

    turn_90_counts = 90*COUNTS_PER_DEGREE;
    turn_180_counts = 180*COUNTS_PER_DEGREE;

    
    // Open file pointer for writing
    fptr = SD.FOpen("RESULTS.txt", "w");
    SD.FPrintf(fptr, "Expected A Position: %f %f %f\n", A_x,A_y,A_heading);
    SD.FPrintf(fptr, "Actual A Position:   %f %f %f\n\n", RPS.X(), RPS.Y(), RPS.Heading());
    // A --> B
    check_y(B_y, PLUS);
    check_heading(B_heading);
    Sleep(1.0);
    

    // COMPLETE CODE HERE TO WRITE EXPECTED AND ACTUAL POSITION INFORMATION TO SD CARD
    SD.FPrintf(fptr, "Expected B Position: %f %f %f\n", B_x,B_y,B_heading);
    SD.FPrintf(fptr, "Actual B Position:   %f %f %f\n\n", RPS.X(), RPS.Y(), RPS.Heading());

     //Log

    // B --> C
    move_forward(POWER, B_C_counts);
    check_x(C_x, MINUS);
    turn_counterclockwise(POWER,  turn_180_counts);
    check_heading(C_heading);
    Sleep(1.0);

    // COMPLETE CODE HERE TO WRITE EXPECTED AND ACTUAL POSITION INFORMATION TO SD CARD
    SD.FPrintf(fptr, "Expected C Position: %f %f %f\n", C_x,C_y,C_heading);
    SD.FPrintf(fptr, "Actual D Position:   %f %f %f\n\n", RPS.X(), RPS.Y(), RPS.Heading());

    // C --> D
    move_forward(POWER, C_D_counts);
    check_x(D_x, PLUS);
    turn_counterclockwise(-POWER, turn_90_counts);
    check_heading(D_heading);
    check_y(D_y, MINUS);
    Sleep(1.0);

    // COMPLETE CODE HERE TO WRITE EXPECTED AND ACTUAL POSITION INFORMATION TO SD CARD
    SD.FPrintf(fptr, "Expected D Position: %f %f %f\n", D_x,D_y,D_heading);
    SD.FPrintf(fptr, "Actual D Position:   %f %f %f\n\n", RPS.X(), RPS.Y(), RPS.Heading());
    // Close file pointer
    SD.FClose(fptr);

    return 0;
}*/
