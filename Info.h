/* 

https://github.com/ilkerya/Essence


For Sending Commands Set-> New Line / 115200 baud
Commands List  // CaseSensitive , only one emptySpace 
Net? // prints current wifi info
Col? // prints Color Codes for Modes
Fan? // prints Fan Duty Cycles for Modes
Ver? // prints compile date &  time 

WIFI SSID PASSWORD // WIFI Neandarthel BerkMertFener // Carefull Both SSID & PASS should be min 4 caharacters!!
Mode  // Switch Betwwen  Test 2 Normal Mode
Key  // Imitates the Key Press
FanH XX  //16-98 as %Duty cycle
FanM XX    //16-98  
FanL XX    //16-98
ColH XXXXXX  //0-FFFFFF  from Colorpalette->  https://www.rapidtables.com/web/color/RGB_Color.html
ColM XXXXXX   //0-FFFFFF  // 6 digit hexadecimal number
ColL XXXXXX   //0-FFFFFF ColL 00bfff blueish
Reset   // System Reset
DSleep  // Goes to sleep wake up by key touch
LSleep  // Goes to sleep wake up by key touch and any serial data sent from terminal 

Time xx xx xx xx xx xxxx //Hour Minute Second Date Month Year !!! Always leading zero if single digit one space or any character in between 
Time Example : 10 04 45 02 11 2026  Year Min 2025 Max 2040

Panel Explanation
Run Mode
MID 2.123
MID  Cycle.Timer 
%FSet:LowSpeed/MidSpeed/HighSpeed
CSet:LowColor/MidColor/HighColor

https://www.rapidtables.com/convert/number/hex-to-decimal.html?x=26CDFF
for clean compilation of the project, delete the folder:
C:\Users\XXXX\AppData\Local\arduino\sketches

//  https://www.ditig.com/256-colors-cheat-sheet
/*
First Time
git init
git add README.md
git add Branch main

GitHub Bash Commands
Adding new file
https://help.github.com/articles/adding-a-file-to-a-repository-using-the-command-line/#platform-windows

git add .
git commit -m "Add existing file"
// For Common git
git push https://github.com/ilkerya/Fragrance_Firmware main/sub

git remote add origin <remote repository URL>
git remote add origin https://github.com/ilkerya/Fragrance_Firmware

In case in the first push gives error use below command
git push origin main
// in case problem use --force option
git push --force https://github.com/ilkerya/Fragrance_Firmware main/sub --force

To create a branch:
$ git branch <branch_name>
To switch to that branch:
$ git checkout <same_branch_name>
To do the above operations in one line, it will create and switch branch:
$ git checkout -b <new_branch>
For example, say you want to create a branch named `learning_git`
$ git branch learning_git
$ git checkout learning_git
or
$ git checkout -b learning_git

C:\Users\XXX\OneDrive - XXX\Documents\Arduino\Fragrance> // path
C:\Users\XXX\OneDrive\ -\ XXX\Documents\Arduino\Fragrance>  // command line

cd OneDrive\ -\ XXX

Free PDF Editor for schematics

https://www.sejda.com/





























*/