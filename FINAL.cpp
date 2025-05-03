#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include<cstring>


using namespace std;
using namespace CryptoPP;


using namespace std;

struct User {
    string username;
    string pass; 
    string email;
    int ID =0;
    string phonenumber;
};

struct Date {
    int day;
    int month;
    int year;
};

struct Charity {
    int charityID;
    string name;
    string description;
    double targetAmount;
    double currentAmount;
    Date deadline;
    string status;
};

void write_file_user(const string file, User user) {
    fstream myfile(file, ios::in | ios::out | ios::app);
    int lastID = 0;

    ifstream infile(file);
    string line, lastLine;

    while (getline(infile, line)) {
        if (!line.empty()) {
            lastLine = line;
        }
    }

    if (!lastLine.empty()) {
        stringstream ss(lastLine);
        ss >> lastID;
    }

    user.ID = lastID + 1;

    myfile << user.ID << "    " << user.email << "   " << user.username << "    " << user.pass << "     " << user.phonenumber << endl;

    myfile.close();
    infile.close();
}

void write_file_charity(const string file, Charity charity) {
    fstream myfile(file, ios::in | ios::out | ios::app);
    int lastID = 0;

    ifstream infile(file);
    string line, lastLine;

    while (getline(infile, line)) {
        if (!line.empty()) {
            lastLine = line;
        }
    }

    if (!lastLine.empty()) {
        stringstream ss(lastLine);
        ss >> lastID;
    }

    charity.charityID = lastID + 1;

    myfile << charity.charityID << "    "
        << charity.name << "   "
        << charity.description << "    "
        << charity.targetAmount << "     "
        << charity.currentAmount << "     "
        << charity.deadline.day << "/" << charity.deadline.month << "/" << charity.deadline.year << "     "
        << charity.status << endl;

    myfile.close();
    infile.close();
}


bool isValidPassword(const string& password) {
    if (password.length() < 8)
        return false;

    bool hasLetter = false, hasDigit = false, hasSpecial = false;

    for (char ch : password) {
        if (isalpha(ch)) hasLetter = true;
        else if (isdigit(ch)) hasDigit = true;
        else if (ispunct(ch)) hasSpecial = true;
    }

    return hasLetter && hasDigit && hasSpecial;
}

bool isValidPhoneNum(const string phone_number)
{
    if (phone_number.length() == 8 && (phone_number.substr(0,2) == "03" || phone_number.substr(0,2) == "08" || phone_number.substr(0, 2) == "81" || phone_number.substr(0, 2) == "70" || phone_number.substr(0, 2) == "71" || phone_number.substr(0, 2) == "78" || phone_number.substr(0, 2) == "76")) {
        return true;
    }
    else {
        return false;
    }
}


string sha1Hash(const string& input) {
    SHA1 sha1;
    string digest;
    StringSource s(input, true,
        new HashFilter(sha1,
            new HexEncoder(
                new StringSink(digest)
            )
        )
    );
    return digest;
}

void SignUp() {
    User user;
    cout << "Please Enter a valid email, username and password" << endl;

    cout << "Email: ";
    cin >> user.email;

    cout << "\nUsername: ";
    cin >> user.username;

    cout << "\nPassword: ";
    cin >> user.pass;

    cout << "\nphone number:";
    cin >> user.phonenumber;

    while (!isValidPassword(user.pass)) {
        cout << "Invalid password. Try again: ";
        cin >> user.pass;
    }
    while (!isValidPhoneNum(user.phonenumber)) {
        cout << "Invalid phone number. Try again: ";
        cin >> user.phonenumber;
    }
    

    //hash the password before storing
    user.pass = sha1Hash(user.pass);
    write_file_user("user.txt", user);
}

void SignIn() {
    User user;
    cout << "enter your username: ";
    cin >> user.username;

    cout << "enter password: ";
    cin >> user.pass;
    string hashedPassword = sha1Hash(user.pass);

        ifstream infile("user.txt");
    string line;

    bool success = false;
    while (getline(infile, line)) {
        stringstream ss(line);
        string id, email, username, password, phonenumber;

        ss >> id >> email >> username >> password >> phonenumber;

        if (username == user.username && password == hashedPassword) {
            success = true;
            break;
        }
    }

    infile.close();
    if (success) {
        cout << "Login successful.\n";
    }
    else {
        cout << "Invalid username or password.\n";
    }

}


void AddCharity() {
    Charity charity;

    cout << "Enter charity name: ";
    cin >> charity.name;

    cin.ignore();

    cout << "Enter description: ";
    getline(cin, charity.description);

    cout << "Enter target amount: ";
    cin >> charity.targetAmount;

    cout << "Enter current amount: ";
    cin >> charity.currentAmount;

    cout << "Enter deadline (day month year): ";
    cin >> charity.deadline.day >> charity.deadline.month >> charity.deadline.year;


    cin.ignore();

    cout << "Enter status: ";
    getline(cin, charity.status);

    write_file_charity("charities.txt", charity);
}


int main()
{
    string Account_answer;

    //Ask user about account existence
    while (true) {
        cout << "Do you have an account or an admin?(Y/N/A)" << endl;
        cin >> Account_answer;

        if (Account_answer == "Y"){
            SignIn();
            break;
        }
        else if (Account_answer == "N") {
            SignUp();
            break;
        }
        else if (Account_answer == "A") {
            SignIn();
            string admin_input;
            cout << "options to do as admin: "<<endl;
            cout << "A: add charity." << endl;
            cout << "B: Remove charity." << endl;
            cout << "C: Modify charity" << endl;

            cin >> admin_input;
            if (admin_input == "A") {

                AddCharity();
            }
            else if (admin_input == "B") {
                //
            }
            else if (admin_input == "C") {
                //
            }
            else {
                cout << "please enter a valid answer as admin: ";
            }


            break;
        }
        else {
            cout << "please enter a valid answer(Y/N)"<<endl;
        }
    }

 

}
