#ifndef WRITEUP_H
#define WRITEUP_H

/**********************
 * Information Screen *
 **********************/

char info_text[] = "In the \"Information\" tab, you will find information on what this project is, how it was made, and who helped create it. In the \"Help\" tab, there are some reequently asked questions that will help you use PortaGuide."

/***********************
 * Information Writeup *
 ***********************/

char inspiration_header_text[] = "Inspiration";
char inspiration_text_text[] = "PortaGuide is roughly based off of an existing product called ArcDroid. ArcDroid is a portable robotic arm that turns a manual plasma cutter into a CNC-enabled one, allowing for precise plasma cutting (for things like brackets) to be done out in the field. For this project, we could not do actual plasma cutting for a variety of reasons, so we chose to focus on the CNC-enabled portable robotic arm portion, with the ability to draw and laser-engrave.";

char capability_header_text[] = "What can PortaGuide do?";
char capability_text_text[] = "PortaGuide is an investigation into portable CNC-capable robots. It is capable of drawing and laser-engraving, with the ability to easily swap between the two attachments. For drawing, any drawing utensil can be used (provided it fits in the pen-holder). It can lso draw on any material you would be able to draw on using that drawing utensil. Laser-engraving is a little bit more particular. Please refer to the manual for specifics.";

char materials_header_text[] = "How was PortaGuide made?";
char materials_text_text[] = "PortaGuide has 4 main components: the mechanical assembly, the GUI, the power distribution PCB, and the controls. The mechanical assembly consists of the rotating assembly, arms, and the mounts for all other components (motors, PCB, etc). The GUI is how the user interacts with the device. The power distribution PCB is in charge of providing power to all motors and MCUs in PortaGuide, as well as charging the built-in battery. And the controls are what conver raw g-code files into controlled movements.";

char people_header_text[] = "Who created PortaGuide?";
char people_text_text[] = "PortaGuide was created by Ryan, Carlos, Edison, and Aidan. Ryan was the one who initially proposed the idea, drawing inspiration from his machinist background. He was primarily in charge of the mechanical design of the robot, and also helped with PCB design and motor controls. Carlos was primarily in charge of the controls for the arm since he is doing his masters in robotics and controls. Edison was primarily in charge of PCB design for our power distribution board, and also helped with some Python scripts to automate some processes. Aidan was primarily in charge of the GUI, and worked with Carlos to integrate it into the controls.";

/****************
 * Help Writeup *
 ****************/

char FAQ_Q1_text[] = "What attachments can I use?";
char FAQ_A1_text[] = "Currently, we have attachments for drawing with a standard sharpie and laser engraving, but these attachments are very easy to switch out for new attachments. It is worth noting that the laser engraver should only be used with appropriate protections in place.";

char FAQ_Q2_text[] = "How do I run one of the demos?";
char FAQ_A2_text[] = "Demos can be run in the \"Demos\" page. We have 6 available for each attachment at this time to demonstrate a variety a different skills and difficulties.";

char FAQ_Q3_text[] = "How do I run a custom program?";
char FAQ_A2_text[] = "For this, you can load a file onto a USB flash drive and plug it into the Raspberry Pi. This will allow you to select from any of the files on the flash drive to run. *Note* This file needs to be a .ngc file that is properly formatted and terminated."

/************
 * Tutorial *
 ************/

char tutorial_text[] = "Tutorial Video";

#endif