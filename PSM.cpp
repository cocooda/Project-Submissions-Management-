#include <iostream>

#include <fstream>

#include <string>

#include <vector>

#include <sstream>

#include <regex>

#include <iomanip>

#include <ctime>

using namespace std;

struct Group {

   int groupNumber;

   vector<string> members;

};

struct Project {

   int projectNumber;

   string description;

   string deadline;

};

struct Submission {

   int groupNumber;

   int projectNumber;

   string submissionDate;

   string state; // "On time" or "Late"

};

vector<Group> groups;

vector<Project> projects;

vector<Submission> submissions;

// Function prototypes

void inputGroupInformation();

void displayGroupInformation();

void displayGroupMembers(int groupNumber);

void saveGroupInfoToFile();

void loadGroupInfoFromFile();

void declareProjects();

void displayProjectInformation();

void submitProject();

void displaySubmissionStatisticsByProject();

void displaySubmissionStatisticsByGroup();

void displayOverallStatistics();

void findLateGroups();

void quit();

int getIntInput(const string& prompt);

string getStringInput(const string& prompt);

string getCurrentDate();

bool projectExists(int projectNumber);

bool groupExists(int groupNumber);

bool submissionExists(int groupNumber, int projectNumber);

bool isValidDateFormat(const string& date);

void printSubmissionStateHeader();

void printSubmissionState(int projectNumber, int groupNumber, const string& state);

string getSubmissionState(int groupNumber, int projectNumber, const string& chosenDate);

int main() {

   int choice;

   do {

       cout << "Menu:\n";

       cout << "1. Group Information\n";

       cout << "2. Project Deadline Information\n";

       cout << "3. Submit Project\n";

       cout << "4. Statistic\n";

       cout << "5. Display Overall Statistic\n";

       cout << "6. Find Groups Incomplete or Late\n";

       cout << "7. Quit\n";

       choice = getIntInput("Enter your choice: ");

       switch (choice) {

       case 1: {

           int subChoice;

           cout << "Group Information:\n";

           cout << "1. Input group information\n";

           cout << "2. Display group information\n";

           cout << "3. Save group information to file\n";

           cout << "4. Load group information from file\n";

           subChoice = getIntInput("Enter your choice: ");

           switch (subChoice) {

           case 1:

               inputGroupInformation();

               break;

           case 2: {

               int displayChoice;

               cout << "1. Display information of all groups\n";

               cout << "2. Display information of a specific group\n";

               displayChoice = getIntInput("Enter your choice: ");

               if (displayChoice == 1) {

                   displayGroupInformation();

               }

               else if (displayChoice == 2) {

                   int groupNumber;

                   groupNumber = getIntInput("Enter group number: ");

                   displayGroupMembers(groupNumber);

               }

               else {

                   cout << "Invalid choice. Please try again.\n";

               }

               break;

           }

           case 3:

               saveGroupInfoToFile();

               break;

           case 4:

               loadGroupInfoFromFile();

               break;

           default:

               cout << "Invalid choice. Please try again.\n";

               break;

           }

           break;

       }

       case 2: {

           int subChoice;

           cout << "Project Deadline Information:\n";

           cout << "1. Declare Projects\n";

           cout << "2. Display Project Information\n";

           subChoice = getIntInput("Enter your choice: ");

           switch (subChoice) {

           case 1:

               declareProjects();

               break;

           case 2:

               displayProjectInformation();

               break;

           default:

               cout << "Invalid choice. Please try again.\n";

               break;

           }

           break;

       }

       case 3:

           submitProject();

           break;

       case 4: {

           char subChoice;

           cout << "a) Display Project Submission State" << endl;

           cout << "b) Display Group Submission State" << endl;

           cout << "Enter your choice (a/b) or any other key to back to menu: ";

           cin >> subChoice;

           subChoice = tolower(subChoice); // convert to lower case to ignore case

           if (subChoice == 'a')

               displaySubmissionStatisticsByProject();

           else if (subChoice == 'b')

               displaySubmissionStatisticsByGroup();

           else

               cout << "Returning to menu." << endl;

           break;

       }

       case 5:

           displayOverallStatistics();

           break;

       case 6:

           findLateGroups();

           break;

       case 7:

           quit();

           break;

       default:

           cout << "Invalid choice. Please try again." << endl;

       }

   } while (choice != 7);

   return 0;

}

int getIntInput(const string& prompt) {

   int input;

   cout << prompt << endl;

   while (!(cin >> input)) {

       cin.clear();

       cin.ignore(10000, '\n');

       cout << "Invalid input. Please enter a number: " << endl;

   }

   cin.ignore(10000, '\n');

   return input;

}

string getStringInput(const string& prompt) {

   string input;

   cout << prompt << endl;

   getline(cin, input);

   return input;

}

string getCurrentDate() {

   time_t t = time(0);

   struct tm now;

   localtime_s(&now, &t); // Use localtime_s for thread-safety

   stringstream ss;

   ss << (now.tm_year + 1900) << '-'

       << setw(2) << setfill('0') << (now.tm_mon + 1) << '-'

       << setw(2) << setfill('0') << now.tm_mday;

   return ss.str();

}

bool projectExists(int projectNumber) {

   for (const auto& project : projects) {

       if (project.projectNumber == projectNumber) {

           return true;

       }

   }

   return false;

}

bool groupExists(int groupNumber) {

   for (const auto& group : groups) {

       if (group.groupNumber == groupNumber) {

           return true;

       }

   }

   return false;

}

bool submissionExists(int groupNumber, int projectNumber) {

   for (const auto& submission : submissions) {

       if (submission.groupNumber == groupNumber && submission.projectNumber == projectNumber) {

           return true;

       }

   }

   return false;

}

bool isValidDateFormat(const string& date) {

   regex datePattern(R"(\d{4}-\d{2}-\d{2})");

   if (!regex_match(date, datePattern)) {

       return false;

   }

   int year, month, day;

   char dash1, dash2;

   stringstream ss(date);

   ss >> year >> dash1 >> month >> dash2 >> day;

   if (dash1 == '-' && dash2 == '-' && month >= 1 && month <= 12 && day >= 1 && day <= 31) {

       // Check for valid number of days in a month

       bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

       if ((month == 2 && day <= (isLeapYear ? 29 : 28)) ||

           (month == 4 && day <= 30) || (month == 6 && day <= 30) ||

           (month == 9 && day <= 30) || (month == 11 && day <= 30) ||

           ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day <= 31)) {

           return true;

       }

   }

   return false;

}

void printSubmissionStateHeader() {

   cout << left << setw(15) << "Project Number"

       << setw(15) << "Group Number"

       << setw(20) << "Submission State" << "\n";

   cout << "-----------------------------------------------------\n";

}

void printSubmissionState(int projectNumber, int groupNumber, const string& state) {

   cout << left << setw(15) << projectNumber

       << setw(15) << groupNumber

       << setw(20) << state << "\n";

}

string getSubmissionState(int groupNumber, int projectNumber, const string& chosenDate) {

   for (const auto& submission : submissions) {

       if (submission.groupNumber == groupNumber && submission.projectNumber == projectNumber) {

           return submission.state;

       }

   }

   const auto& project = *find_if(projects.begin(), projects.end(), [&](const Project& proj) { return proj.projectNumber == projectNumber; });

   return (chosenDate <= project.deadline) ? "Not yet submitted" : "Not submitted";

}

void inputGroupInformation() {

   int numGroups = getIntInput("Enter the number of groups: ");

   vector<string> allMembers; // Vector to store all members encountered so far

   for (int i = 0; i < numGroups; ++i) {

       Group group;

       group.groupNumber = i + 1;

       int numMembers;

       do {

           numMembers = getIntInput("Enter the number of members for group " + to_string(i + 1) + " (should be at least 1): ");

           if (numMembers < 1) {

               cout << "At least one member is required for each group.\n";

           }

       } while (numMembers < 1);

       for (int j = 0; j < numMembers; ++j) {

           string member;

           bool memberExists;

           do {

               member = getStringInput("Enter member " + to_string(j + 1) + " name: ");

               memberExists = false;

               // Check if the member already exists in any existing group

               for (const auto& existingMember : allMembers) {

                   if (existingMember == member) {

                       memberExists = true;

                       cout << member << " already belongs to another group. Please enter a different name.\n";

                       break;

                   }

               }

           } while (memberExists);

           group.members.push_back(member);

           allMembers.push_back(member); // Add the member to the vector of all members

       }

       groups.push_back(group);

   }

}

void displayGroupInformation() {

   cout << "Group Information:" << endl;

   for (const auto& group : groups) {

       cout << "Group " << group.groupNumber << " Members:" << endl;

       for (const auto& member : group.members) {

           cout << member << endl;

       }

       cout << endl;

   }

}

void displayGroupMembers(int groupNumber) {

   for (const auto& group : groups) {

       if (group.groupNumber == groupNumber) {

           cout << "Group " << group.groupNumber << " Members:" << endl;

           for (const auto& member : group.members) {

               cout << member << endl;

           }

           return;

       }

   }

   cout << "Group " << groupNumber << " not found." << endl;

}

void saveGroupInfoToFile() {

   ofstream outFile("group.txt");

   if (!outFile) {

       cout << "Error: Unable to open file for writing." << endl;

       return;

   }

   for (const auto& group : groups) {

       outFile << "Group " << group.groupNumber << " Members:" << endl;

       for (const auto& member : group.members) {

           outFile << member << endl;

       }

       outFile << endl;

   }

   outFile.close();

   cout << "Group information saved to file successfully." << endl;

}

void loadGroupInfoFromFile() {

   ifstream inFile("group.txt");

   if (!inFile) {

       cout << "Error: Unable to open file for reading." << endl;

       return;

   }

   groups.clear(); // Clear existing group information

   string line;

   Group group;

   while (getline(inFile, line)) {

       if (line.find("Group ") != string::npos) {

           if (!group.members.empty()) {

               groups.push_back(group);

               group.members.clear();

           }

           group.groupNumber = stoi(line.substr(6, line.find(" Members:") - 6));

       }

       else if (!line.empty()) {

           group.members.push_back(line);

       }

   }

   if (!group.members.empty()) {

       groups.push_back(group);

   }

   inFile.close();

   cout << "Group information loaded from file successfully." << endl;

}

void declareProjects() {

   int numProjects = getIntInput("Enter the number of projects: ");

   for (int i = 0; i < numProjects; ++i) {

       Project project;

       project.projectNumber = i + 1;

       project.description = getStringInput("Enter the description of project " + to_string(i + 1) + ": ");

       string deadline;

       bool validDate = false;

       while (!validDate) {

           deadline = getStringInput("Enter the deadline (YYYY-MM-DD) for project " + to_string(i + 1) + ": ");

           if (isValidDateFormat(deadline)) {

               validDate = true;

           }

           else {

               cout << "Invalid date format. Please enter the date in YYYY-MM-DD format.\n";

           }

       }

       project.deadline = deadline;

       projects.push_back(project);

   }

}

void displayProjectInformation() {

   cout << "Project Information:" << endl;

   for (const auto& project : projects) {

       cout << "Project " << project.projectNumber << ":" << endl;

       cout << "Description: " << project.description << endl;

       cout << "Deadline: " << project.deadline << endl;

       cout << endl;

   }

}

void submitProject() {

   for (auto& project : projects) { // Loop through all projects

       cout << "Project " << project.projectNumber << " submission:\n";

       cout << "Do you want to skip submission for this project? (Y/N): ";

       char projectChoice;

       cin >> projectChoice;

       cin.ignore();

       if (projectChoice == 'Y' || projectChoice == 'y') continue; // Skip this project if 'Y' or 'y' is entered

       bool allSubmitted = false;

       while (!allSubmitted) {

           int groupNumber = getIntInput("Enter group number: ");

           if (!groupExists(groupNumber)) {

               cout << "Group number does not exist. Do you want to input again or move on to the next project? (Y/N): ";

               char choice;

               cin >> choice;

               cin.ignore();

               if (choice == 'N' || choice == 'n') break;

               continue;

           }

           // Check if the group has already submitted this project

           if (submissionExists(groupNumber, project.projectNumber)) {

               cout << "This group has already submitted for this project. You can't change the data.\n";

               continue;

           }

           string submissionDate = getStringInput("Enter submission date for Group " + to_string(groupNumber) + " (format: YYYY-MM-DD): ");

           if (!isValidDateFormat(submissionDate)) {

               cout << "Submission date format is incorrect. Please enter a valid date.\n";

               continue;

           }

           string state = (submissionDate <= project.deadline) ? "On time" : "Late";

           submissions.push_back({ groupNumber, project.projectNumber, submissionDate, state });

           allSubmitted = true;

           for (const auto& group : groups) {

               if (!submissionExists(group.groupNumber, project.projectNumber)) {

                   allSubmitted = false;

                   break;

               }

           }

           if (allSubmitted) {

               cout << "All groups have submitted for this project.\n";

           }

           else {

               cout << "Not all groups have submitted for this project. Continue with the next group? (Y/N): ";

               char choice;

               cin >> choice;

               cin.ignore();

               if (choice == 'N' || choice == 'n') break;

           }

       }

   }

}

void displaySubmissionStatisticsByProject() {

   int projectNumber;

   while (true) {

       projectNumber = getIntInput("Enter project number to display statistics (or -1 to quit): ");

       if (projectNumber == -1) return;

       if (!projectExists(projectNumber)) {

           cout << "Project number does not exist. Please enter a valid project number.\n";

           continue;

       }

       printSubmissionStateHeader();

       for (const auto& group : groups) {

           string state = getSubmissionState(group.groupNumber, projectNumber, getCurrentDate());

           printSubmissionState(projectNumber, group.groupNumber, state);

       }

       if (getStringInput("Do you want to display statistics for another project? (Y/N): ") == "N") break;

   }

}

void displaySubmissionStatisticsByGroup() {

   int groupNumber;

   while (true) {

       groupNumber = getIntInput("Enter group number to display statistics (or -1 to quit): ");

       if (groupNumber == -1) return;

       if (!groupExists(groupNumber)) {

           cout << "Group number does not exist. Please enter a valid group number.\n";

           continue;

       }

       printSubmissionStateHeader();

       for (const auto& project : projects) {

           string state = getSubmissionState(groupNumber, project.projectNumber, getCurrentDate());

           printSubmissionState(project.projectNumber, groupNumber, state);

       }

       if (getStringInput("Do you want to display statistics for another group? (Y/N): ") == "N") break;

   }

}

void displayOverallStatistics() {

   string chosenDate;

   char dateChoice = getStringInput("Do you want to enter a date or use the current date? (E for enter, C for current): ")[0];

   if (dateChoice == 'E' || dateChoice == 'e') {

       do {

           chosenDate = getStringInput("Enter the date (YYYY-MM-DD): ");

           if (!isValidDateFormat(chosenDate)) {

               cout << "Invalid date format. Please enter the date in YYYY-MM-DD format.\n";

           }

       } while (!isValidDateFormat(chosenDate));

       if (chosenDate > getCurrentDate()) {

           cout << "The chosen date is after current time. " << endl;

           cout << "We will just get statistic to current time." << endl;

           chosenDate = getCurrentDate();

       }

   }

   else {

       chosenDate = getCurrentDate();

   }

   ofstream outFile("OverallStatistic.txt");

   if (!outFile) {

       cerr << "Error: Unable to open file for writing." << endl;

       return;

   }

   cout << "Overall Statistics as of " << chosenDate << ":\n";

   outFile << "Overall Statistics as of " << chosenDate << ":\n";

   printSubmissionStateHeader();

   outFile << left << setw(15) << "Project Number"

       << setw(15) << "Group Number"

       << setw(20) << "Submission State" << "\n";

   outFile << "-----------------------------------------------------\n";

   for (const auto& project : projects) {

       if (project.deadline < chosenDate) {

           for (const auto& group : groups) {

               string state = getSubmissionState(group.groupNumber, project.projectNumber, chosenDate);

               printSubmissionState(project.projectNumber, group.groupNumber, state);

               outFile << left << setw(15) << project.projectNumber

                   << setw(15) << group.groupNumber

                   << setw(20) << state << "\n";

           }

           cout << "-----------------------------------------------------\n";

           outFile << "-----------------------------------------------------\n";

       }

   }

   outFile.close();

   cout << "Overall statistics have been exported to OverallStatistic.txt.\n";

}

void findLateGroups() {

   cout << "Groups that did not complete or submit projects on time:" << endl;

   for (const auto& group : groups) {

       bool incompleteOrLate = false;

       for (const auto& project : projects) {

           string state = getSubmissionState(group.groupNumber, project.projectNumber, getCurrentDate());

           if (state == "Not submitted" || state == "Not yet submitted" || state == "Late") {

               incompleteOrLate = true;

               break; // No need to check further projects for this group

           }

       }

       if (incompleteOrLate) {

           cout << "Group " << group.groupNumber << " has not completed or has late submissions." << endl;

       }

   }

}

void quit() {

   cout << "Quitting program..." << endl;

}
