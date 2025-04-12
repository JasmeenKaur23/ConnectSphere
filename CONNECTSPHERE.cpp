#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <regex>
#include <map>

using namespace std;

// Forward declarations
class Contact;
class PersonalContact;
class BusinessContact;
class FamilyContact;
class FileOperations;
class User;
class SecurityQuestion;

// SecurityQuestion class for password reset
class SecurityQuestion {
public:
    string question;
    string answer;

    SecurityQuestion(const string& q = "", const string& a = "") : question(q), answer(a) {}
};

// User class with login and password reset functionality
class User {
private:
    string username;
    string password;
    SecurityQuestion securityQuestion;

public:
    User(const string& uname = "", const string& pword = "", const SecurityQuestion& secQ = SecurityQuestion())
        : username(uname), password(pword), securityQuestion(secQ) {}

    bool authenticate(const string& uname, const string& pword) const {
        return (username == uname && password == pword);
    }

    bool resetPassword(const string& answer, const string& newPass) {
        if (securityQuestion.answer == answer) {
            password = newPass;
            return true;
        }
        return false;
    }

    const string& getUsername() const { return username; }
    const string& getPassword() const { return password; }
    const string& getSecurityQuestion() const { return securityQuestion.question; }
    const string& getSecurityAnswer() const { return securityQuestion.answer; }

    // Serialize user data for saving to file
    string serialize() const {
        return username + "," + password + "," + securityQuestion.question + "," + securityQuestion.answer;
    }

    // Deserialize user data from file
    static User deserialize(const string& data) {
        stringstream ss(data);
        string uname, pword, question, answer;
        getline(ss, uname, ',');
        getline(ss, pword, ',');
        getline(ss, question, ',');
        getline(ss, answer, ',');
        return User(uname, pword, SecurityQuestion(question, answer));
    }
};

// Base class for contacts


// Base class for contacts
class Contact {
    public:
        string name;
        string phone;
    
        Contact() {}
        Contact(const string& name, const string& phone) : name(name), phone(phone) {}
    
        virtual void display() = 0;
        virtual void share() = 0; // Share functionality
        virtual bool validate() const = 0;
    };
    
    // PersonalContact class
    class PersonalContact : public Contact {
    public:
        PersonalContact() {}
        PersonalContact(const string& name, const string& phone) : Contact(name, phone) {}
    
        void display() override {
            cout << "Personal Contact: Name - " << name << ", Phone - " << phone << endl;
        }
    
        void share() override {
            ofstream file("shared_contact.txt", ios::app); // Append to file
            if (file.is_open()) {
                file << "Personal Contact: Name - " << name << ", Phone - " << phone << "\n";
                file.close();
                cout << "Contact shared successfully! Saved to 'shared_contact.txt'." << endl;
            } else {
                cout << "Error: Unable to share contact." << endl;
            }
        }
    
        bool validate() const override {
            regex phoneRegex(R"(\d{10})");
            return regex_match(phone, phoneRegex);
        }
    };
    
    // BusinessContact class
    class BusinessContact : public Contact {
    public:
        string company;
    
        BusinessContact() {}
        BusinessContact(const string& name, const string& phone, const string& company)
            : Contact(name, phone), company(company) {}
    
        void display() override {
            cout << "Business Contact: Name - " << name << ", Phone - " << phone << ", Company - " << company << endl;
        }
    
        void share() override {
            ofstream file("shared_contact.txt", ios::app); // Append to file
            if (file.is_open()) {
                file << "Business Contact: Name - " << name << ", Phone - " << phone << ", Company - " << company << "\n";
                file.close();
                cout << "Contact shared successfully! Saved to 'shared_contact.txt'." << endl;
            } else {
                cout << "Error: Unable to share contact." << endl;
            }
        }
    
        bool validate() const override {
            regex phoneRegex(R"(\d{10})");
            return (regex_match(phone, phoneRegex) && !company.empty());
        }
    };
    
    // FamilyContact class
    class FamilyContact : public Contact {
    public:
        string relation;
    
        FamilyContact() {}
        FamilyContact(const string& name, const string& phone, const string& relation)
            : Contact(name, phone), relation(relation) {}
    
        void display() override {
            cout << "Family Contact: Name - " << name << ", Phone - " << phone << ", Relation - " << relation << endl;
        }
    
        void share() override {
            ofstream file("shared_contact.txt", ios::app); // Append to file
            if (file.is_open()) {
                file << "Family Contact: Name - " << name << ", Phone - " << phone << ", Relation - " << relation << "\n";
                file.close();
                cout << "Contact shared successfully! Saved to 'shared_contact.txt'." << endl;
            } else {
                cout << "Error: Unable to share contact." << endl;
            }
        }
    
        bool validate() const override {
            regex phoneRegex(R"(\d{10})");
            return (regex_match(phone, phoneRegex) && !relation.empty());
        }
    };
//////////////

// File operations interface
class FileOperations {
public:
    virtual void saveToFile(const string& filename) = 0;
    virtual void loadFromFile(const string& filename) = 0;
    virtual vector<string> getFileHistory() = 0;
    virtual ~FileOperations() {}
};

// Phonebook class
class Phonebook : public FileOperations {
public:
    vector<Contact*> contacts;
    map<string, vector<Contact*>> contactGroups;
    vector<string> fileHistory;



    
    const vector<Contact*>& getContacts() const {
        return contacts;
    }

  
    void addContact(Contact* contact, const string& group) {
        if (contact->validate()) {
            contacts.push_back(contact);
            contactGroups[group].push_back(contact);
            fileHistory.push_back("Added " + contact->name + " (" + contact->phone + ") to group '" + group + "'.");
            cout << "Contact added successfully!" << endl;
        } else {
            cout << "Invalid contact details. Contact not added." << endl;
            delete contact;
        }
    }

    void displayContacts() {
        if (contacts.empty()) {
            cout << "No contacts available." << endl;
            return;
        }
        cout << "Contacts:" << endl;
        for (size_t i = 0; i < contacts.size(); ++i) {
            cout << i + 1 << ". " << contacts[i]->name << " (" << contacts[i]->phone << ")" << endl;
        }
    }

    void shareContact(int index) {
        if (index >= 1 && static_cast<size_t>(index) <= contacts.size()) {
            contacts[index - 1]->share();
        } else {
            cout << "Invalid contact number." << endl;
        }
    }

    void displayGroup(const string& group) {
        if (contactGroups.find(group) != contactGroups.end() && !contactGroups[group].empty()) {
            cout << "Contacts in Group - " << group << ":" << endl;
            for (size_t i = 0; i < contactGroups[group].size(); ++i) {
                cout << i + 1 << ". " << contactGroups[group][i]->name << " (" << contactGroups[group][i]->phone << ")" << endl;
            }
        } else {
            cout << "Group not found or empty." << endl;
        }
    }

    void saveToFile(const string& filename) override {
        ofstream file(filename);
        if (file.is_open()) {
            for (Contact* contact : contacts) {
                if (auto* pc = dynamic_cast<PersonalContact*>(contact)) {
                    file << "P," << pc->name << "," << pc->phone << "\n";
                } else if (auto* bc = dynamic_cast<BusinessContact*>(contact)) {
                    file << "B," << bc->name << "," << bc->phone << "," << bc->company << "\n";
                } else if (auto* fc = dynamic_cast<FamilyContact*>(contact)) {
                    file << "F," << fc->name << "," << fc->phone << "," << fc->relation << "\n";
                }
            }
            file.close();
            fileHistory.push_back("Saved: " + filename);
        } else {
            cout << "Error: Unable to open the file for writing." << endl;
        }

        // Save contact groups
        ofstream groupFile("groups.txt");
        if (groupFile.is_open()) {
            for (const auto& group : contactGroups) {
                for (size_t i = 0; i < group.second.size(); ++i) {
                    for (size_t j = 0; j < contacts.size(); ++j) {
                        if (contacts[j] == group.second[i]) {
                            groupFile << group.first << "," << j << "\n";
                            break;
                        }
                    }
                }
            }
            groupFile.close();
        }
    }

    void loadFromFile(const string& filename) override {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string type, name, phone, extra;
                if (getline(ss, type, ',') && getline(ss, name, ',') && getline(ss, phone, ',')) {
                    if (type == "P") {
                        Contact* contact = new PersonalContact(name, phone);
                        if (contact->validate()) {
                            contacts.push_back(contact);
                        } else {
                            delete contact;
                        }
                    } else if (type == "B" && getline(ss, extra, ',')) {
                        Contact* contact = new BusinessContact(name, phone, extra);
                        if (contact->validate()) {
                            contacts.push_back(contact);
                        } else {
                            delete contact;
                        }
                    } else if (type == "F" && getline(ss, extra, ',')) {
                        Contact* contact = new FamilyContact(name, phone, extra);
                        if (contact->validate()) {
                            contacts.push_back(contact);
                        } else {
                            delete contact;
                        }
                    }
                }
            }
            file.close();
            fileHistory.push_back("Loaded: " + filename);
        } else {
            cout << "No existing phonebook file found. Starting fresh." << endl;
        }

        // Load contact groups
        ifstream groupFile("groups.txt");
        if (groupFile.is_open()) {
            string line;
            while (getline(groupFile, line)) {
                stringstream ss(line);
                string groupName;
                int index;
                if (getline(ss, groupName, ',') && ss >> index) {
                    if (index >= 0 && static_cast<size_t>(index) < contacts.size()) {
                        contactGroups[groupName].push_back(contacts[index]);
                    }
                }
            }
            groupFile.close();
        }
    }

    vector<string> getFileHistory() override {
        return fileHistory;
    }


    void searchContact(const string& query) {
        bool found = false;
        for (size_t i = 0; i < contacts.size(); ++i) {
            if (contacts[i]->name.find(query) != string::npos || contacts[i]->phone.find(query) != string::npos) {
                cout << i + 1 << ". ";
                contacts[i]->display();
                found = true;
            }
        }
        if (!found) {
            cout << "No contacts found matching '" << query << "'." << endl;
        }
    }

    
    void deleteContact(int index) {
        if (index >= 1 && static_cast<size_t>(index) <= contacts.size()) {
            Contact* contactToDelete = contacts[index - 1];
            fileHistory.push_back("Deleted " + contactToDelete->name + " (" + contactToDelete->phone + ").");
            contacts.erase(contacts.begin() + (index - 1));
            for (auto& group : contactGroups) {
                group.second.erase(
                    remove(group.second.begin(), group.second.end(), contactToDelete),
                    group.second.end()
                );
            }
            delete contactToDelete;
            cout << "Contact deleted successfully." << endl;
        } else {
            cout << "Invalid contact number." << endl;
        }
    }

    ~Phonebook() {
        for (Contact* contact : contacts) {
            delete contact;
        }
    }
};

// Function to load users from file
map<string, User> loadUsers(const string& filename) {
    map<string, User> users;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            User user = User::deserialize(line);
            users[user.getUsername()] = user;
        }
        file.close();
    }
    return users;
}

// Function to save users to file
void saveUsers(const string& filename, const map<string, User>& users) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : users) {
            file << pair.second.serialize() << "\n";
        }
        file.close();
    }
}

// Function to wait for user to press Enter
void pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// Main function
int main() {
    string userFile = "users.txt";
    string phonebookFile = "phonebook.txt";
    map<string, User> users = loadUsers(userFile);
    string currentUser;

    // Login or register
    bool loggedIn = false;
    while (!loggedIn) {
        system("cls");
        cout << "1. Login\n2. Register\nEnter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string username, password;
            cout << "Enter Username: ";
            getline(cin, username);
            cout << "Enter Password: ";
            getline(cin, password);

            if (users.find(username) != users.end() && users[username].authenticate(username, password)) {
                loggedIn = true;
                currentUser = username;
                cout << "Login successful!" << endl;
                pressEnterToContinue();
            } else {
                cout << "Login failed! Please try again." << endl;
                pressEnterToContinue();
            }
        } else if (choice == 2) {
            string username, password, question, answer;
            cout << "Enter Username: ";
            getline(cin, username);
            cout << "Enter Password: ";
            getline(cin, password);
            cout << "Enter Security Question: ";
            getline(cin, question);
            cout << "Enter Answer: ";
            getline(cin, answer);

            if (users.find(username) == users.end()) {
                users[username] = User(username, password, SecurityQuestion(question, answer));
                saveUsers(userFile, users);
                cout << "Registration successful! Please login." << endl;
                pressEnterToContinue();
            } else {
                cout << "Username already exists. Please try again." << endl;
                pressEnterToContinue();
            }
        }
    }

    // Phonebook operations
    Phonebook phonebook;
    phonebook.loadFromFile(phonebookFile);

    int choice;
    do {
        system("cls");
        cout << "\t\t\t|********************************************|\n";
        cout << "\t\t\t|           CONNECTSPHERE                    | \n";
        cout << "\t\t\t**********************************************\n\n";
        cout << "\t\t\t::::::::::::::: PROGRAM MENU :::::::::::::::\n" << endl;
        cout << "\t\t\t**********************************************************" << endl;
        cout << "\t\t\t|             [1] Add Personal Contact                   |" << endl;
        cout << "\t\t\t|             [2] Add Business Contact                   |" << endl;
        cout << "\t\t\t|             [3] Add Family Contact                     |" << endl;
        cout << "\t\t\t|             [4] Display Contacts                       |" << endl;
        cout << "\t\t\t|             [5] Share Contact                          |" << endl;
        cout << "\t\t\t|             [6] Save Phonebook                         |" << endl;
        cout << "\t\t\t|             [7] Create Contact Group                   |" << endl;
        cout << "\t\t\t|             [8] Display Group                          |" << endl;
        cout << "\t\t\t|             [9] Password Reset                         |" << endl;
        cout << "\t\t\t|             [10] View File History                     |" << endl;
        cout << "\t\t\t|             [11] Delete Contact                        |" << endl;
        cout << "\t\t\t|             [12] Search Contact                        |" << endl;
        cout << "\t\t\t|             [13] Quit                                  |" << endl;
        cout << "\t\t\t**********************************************************" << endl;
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                cout << "\t\t\t**********************************************\n\n";
                string name, phone;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Phone (10 digits): ";
                getline(cin, phone);
                if (!regex_match(phone, regex(R"(\d{10})"))) {
                    cout << "Invalid phone number format. Please enter 10 digits." << endl;
                } else {
                    Contact* contact = new PersonalContact(name, phone);
                    phonebook.addContact(contact, "Personal");
                }
                pressEnterToContinue();
                break;
            }
            case 2: {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                cout << "\t\t\t**********************************************\n\n";
                string name, phone, company;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Phone (10 digits): ";
                getline(cin, phone);
                if (!regex_match(phone, regex(R"(\d{10})"))) {
                    cout << "Invalid phone number format. Please enter 10 digits." << endl;
                } else {
                    cout << "Enter Company: ";
                    getline(cin, company);
                    if (company.empty()) {
                        cout << "Company name cannot be empty. Please try again." << endl;
                    } else {
                        Contact* contact = new BusinessContact(name, phone, company);
                        phonebook.addContact(contact, "Business");
                    }
                }
                pressEnterToContinue();
                break;
            }
            case 3: {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
              cout << "\t\t\t|           CONNECTSPHERE                    | \n";
              cout << "\t\t\t**********************************************\n\n";
                string name, phone, relation;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Phone (10 digits): ";
                getline(cin, phone);
                if (!regex_match(phone, regex(R"(\d{10})"))) {
                    cout << "Invalid phone number format. Please enter 10 digits." << endl;
                } else {
                    cout << "Enter Relation: ";
                    getline(cin, relation);
                    if (relation.empty()) {
                        cout << "Relation cannot be empty. Please try again." << endl;
                    } else {
                        Contact* contact = new FamilyContact(name, phone, relation);
                        phonebook.addContact(contact, "Family");
                    }
                }
                pressEnterToContinue();
                break;
            }
            case 4:
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                cout << "\t\t\t**********************************************\n\n";
                phonebook.displayContacts();
                pressEnterToContinue();
                break;
            case 5: {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                cout << "\t\t\t**********************************************\n\n";
                phonebook.displayContacts();
                int index;
                cout << "Enter the number of the contact to share: ";
                cin >> index;
                phonebook.shareContact(index);
                pressEnterToContinue();
                break;
            }
            case 6:
            {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                cout << "\t\t\t**********************************************\n\n";
                phonebook.saveToFile(phonebookFile);
                pressEnterToContinue();
                break;
        }
                case 7: {
                    system("cls");
                    cout << "\t\t\t|********************************************|\n";
                    cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                    cout << "\t\t\t**********************************************\n\n";
                    phonebook.displayContacts(); // Display all contacts
                    string group;
                    cin.ignore();
                    cout << "Enter the name of the new contact group: ";
                    getline(cin, group);
                
                    if (!group.empty()) {
                        phonebook.contactGroups[group] = vector<Contact*>(); // Create a new group
                        cout << "Contact group '" << group << "' created." << endl;
                
                        // Add contacts to the group
                        cout << "Enter the numbers of the contacts to add to this group (separated by spaces, e.g., 1 2 3): ";
                        string input;
                        getline(cin, input);
                        stringstream ss(input);
                        int index;
                        while (ss >> index) {
                            if (index >= 1 && static_cast<size_t>(index) <= phonebook.getContacts().size()) {
                                phonebook.contactGroups[group].push_back(phonebook.getContacts()[index - 1]);
                                cout << "Contact " << phonebook.getContacts()[index - 1]->name << " added to group '" << group << "'." << endl;
                            } else {
                                cout << "Invalid contact number: " << index << endl;
                            }
                        }
                    } else {
                        cout << "Group name cannot be empty. Please try again." << endl;
                    }
                    pressEnterToContinue();
                    break;
                }
            
                case 8: {
                    system("cls");
                    cout << "\t\t\t|********************************************|\n";
                    cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                    cout << "\t\t\t**********************************************\n\n";
                
                    // Display all contact groups
                    if (phonebook.contactGroups.empty()) {
                        cout << "No contact groups available." << endl;
                    } else {
                        cout << "Contact Groups:" << endl;
                        int groupNumber = 1;
                        for (const auto& group : phonebook.contactGroups) {
                            cout << groupNumber << ". " << group.first << endl;
                            groupNumber++;
                        }
                
                        // Allow the user to select a group to view its contacts
                        cout << "Enter the number of the group to view: ";
                        int selectedGroup;
                        cin >> selectedGroup;
                
                        if (selectedGroup >= 1 && selectedGroup <= phonebook.contactGroups.size()) {
                            auto it = phonebook.contactGroups.begin();
                            advance(it, selectedGroup - 1); // Move iterator to the selected group
                            cout << "Contacts in Group - " << it->first << ":" << endl;
                            for (size_t i = 0; i < it->second.size(); ++i) {
                                cout << i + 1 << ". " << it->second[i]->name << " (" << it->second[i]->phone << ")" << endl;
                            }
                
                            // Allow the user to move a contact to another group
                            cout << "Enter the number of the contact to move (or 0 to cancel): ";
                            int contactToMove;
                            cin >> contactToMove;
                
                            if (contactToMove >= 1 && contactToMove <= it->second.size()) {
                                cout << "Enter the name of the target group: ";
                                string targetGroup;
                                cin.ignore();
                                getline(cin, targetGroup);
                
                                if (!targetGroup.empty()) {
                                    // Move the contact to the target group
                                    Contact* contact = it->second[contactToMove - 1];
                                    phonebook.contactGroups[targetGroup].push_back(contact);
                                    it->second.erase(it->second.begin() + (contactToMove - 1));
                                    cout << "Contact " << contact->name << " moved to group '" << targetGroup << "'." << endl;
                                } else {
                                    cout << "Group name cannot be empty. Please try again." << endl;
                                }
                            } else if (contactToMove != 0) {
                                cout << "Invalid contact number." << endl;
                            }
                        } else {
                            cout << "Invalid group number." << endl;
                        }
                    }
                    pressEnterToContinue();
                    break;
                }
            case 9: {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                cout << "\t\t\t**********************************************\n\n";
                string answer, newPassword;
                cin.ignore();
                cout << "Security Question: " << users[currentUser].getSecurityQuestion() << endl;
                cout << "Enter the answer to reset your password: ";
                getline(cin, answer);
                cout << "Enter your new password: ";
                getline(cin, newPassword);
                if (users[currentUser].resetPassword(answer, newPassword)) {
                    cout << "Password reset successfully." << endl;
                    saveUsers(userFile, users);
                } else {
                    cout << "Password reset failed. Security answer is incorrect." << endl;
                }
                pressEnterToContinue();
                break;
            }
            case 10: {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|            CONNECTSPHERE                   | \n";
                cout << "\t\t\t**********************************************\n\n";
                vector<string> history = phonebook.getFileHistory();
                cout << "File History (Operations Performed in This Session):" << endl;
                if (history.empty()) {
                    cout << "No operations performed yet." << endl;
                } else {
                    for (const string& entry : history) {
                        cout << entry << endl;
                    }
                }
                pressEnterToContinue();
                break;
            }
            case 11: {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|               CONNECTSPHERE                | \n";
                cout << "\t\t\t**********************************************\n\n";
                phonebook.displayContacts();
                int index;
                cout << "Enter the number of the contact to delete: ";
                cin >> index;
                phonebook.deleteContact(index);
                pressEnterToContinue();
                break;
            }

            case 12: {
                system("cls");
                cout << "\t\t\t|********************************************|\n";
                cout << "\t\t\t|           CONNECTSPHERE                    | \n";
                cout << "\t\t\t**********************************************\n\n";
                string query;
                cin.ignore();
                cout << "Enter name or phone to search: ";
                getline(cin, query);
                phonebook.searchContact(query);
                pressEnterToContinue();
                break;
            }
            case 13:
            system("cls");
            cout << "\t\t\t|********************************************|\n";
            cout << "\t\t\t|              CONNECTSPHERE                 | \n";
            cout << "\t\t\t**********************************************\n\n";
            phonebook.saveToFile(phonebookFile);
            saveUsers(userFile, users);
            cout << "Phonebook and user data saved." << endl;
            cout << "Goodbye!" << endl;
            pressEnterToContinue();
            break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                pressEnterToContinue();
        }
    } while (choice != 12);

    // Clean up dynamic memory
    for (Contact* contact : phonebook.getContacts()) {
        delete contact;
    }

    return 0;
}