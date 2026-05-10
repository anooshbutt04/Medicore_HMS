# Medicore_HMS
OVERVIEW
Medicore is a completely functional Hospital Management System built with C++ and designed with SFML Graphical Interface. The console serves as my Final Term Project for OOP course.The console supports three roles- Patient, Doctor and Admin, namely and manages bills, appointments, prescriptions, records, and security etc. All data that is required for basic information and initial record is shared across the program via a .txt file. 

______________________________________________________________________________

FEATURES
Role              Capabilities
Patient:        Book/Cancel Appointments, Pay Bills, View Medical Record, 
                      View Top-Up Balance.

Doctor:        View Appointments, Mark Appointments as Complete, View
                     Patients’ Records, Write Prescription

Admin:        Add/Remove Doctors, Discharge Patient(s), View All Data,
                     Generate Daily Report,View Security Log

After three (3) failed login attempts, the account is locked and update is logged into
security_log.txt file. 

______________________________________________________________________________

DEPENDENCIES

C++
SFML 2.5+ 
-For Windows, download from ( https://www.sfml-dev.org/download.php )
________________________________________________________________________

How to Compile & Run
Visual Studio (Windows) — Recommended
Create a new Empty C++ Project.
Add all .h and .cpp files to the project.
Link SFML:
Go to Project → Properties → C/C++ → Additional Include Directories → add the SFML include/ path.
Go to Linker → Additional Library Directories → add the SFML lib/ path.
Go to Linker → Input → Additional Dependencies → add:
    sfml-graphics.lib
    sfml-window.lib
    sfml-system.lib
    sfml-audio.lib
Copy the SFML .dll files into your project's output folder (same folder as the .exe).
Make sure the assets/ folder and all .txt data files are in the same directory as the executable.
Press Ctrl+F5 / F5 to build and run.
__________________________________________________________

ADDITIONAL NOTES
Std::string or any built in string functions are not used or implemented anywhere in the code logic due to the constraints added in the assignment. They are only employed in the SFML/GUI code.
Std::vector is also only implemented in the GUI implementation as permitted.
There are no memory leakages in the program.
