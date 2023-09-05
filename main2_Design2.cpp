#include <iostream>
#include <limits> //Needed to use limit of stream size while cleaning the buffer
#include <cstring> //Needed to use embedded string functions like strcmp
using namespace std;
int Number_of_accounts=0; //Counter of accounts


//CLASS DEFINITION
class ATM {
    private: //Defining private members of the class
        char Account_name[101]; //Account name + NULL terminator
        char Card_number[11]; //Card number + NULL terminator
        float Amount;
    public: //Declaring and defining public members of the class (functions only)
        void create_new_acc();
        void Deposit(float deposit_amount);
        void Transfer(int rec,float trans);
        void Print_acc_data();
        void Withdrawal(float with_amount);
        int index_of_account(char number[]);
        bool found(char number[]);
        float get_amount(){ //Function to return amount
            return Amount;
        }
        ATM (){
            Amount=0;
        }
        //Some declared functions is to be defined later
};

//ARRAY DECLARATION
ATM Accounts[100];

//NUMBER VALIDATION FUNCTION
bool validate_number(char num[]){ //Will be used to validate that any account number is satisfying the conditions
            for(int counter=0;counter<strlen(num);counter++){
                if(num[counter]==32){ //Separation using spaces is not allowed
                    cout<<"Please, enter a valid number (spaces are not allowed): ";
                    return false;
                }
                if(!(num[counter]>=48 && num[counter]<=57)){ //Make sure it has only numerical characters
                  cout<<"Please, enter a valid number (only numbers from 0 to 9 are accepted): ";
                  return false;
                }
            }
            if(!cin){ //Checking failure of cin (long input for example)
                cin.clear(); //Exiting cin failure mode
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
                cout<<"Please, enter a number of 10 digits only: ";
                return false;
            }
            if(strlen(num)<10){ //Checking the length
                cout<<"Please, enter a number of at least 10 digits: ";
                return false;
            }
            return true;
}

//CHECK MONEY FUNCTION
bool makesure(){ //Will be used for checking if any account is valid for withdrawal or transfer
            for(int counter=0;counter<Number_of_accounts;counter++){
                if(Accounts[counter].get_amount()>0){
                    return 1;
                }
            }
            return 0;
        }

//SEARCH FUNCTION
int ATM :: index_of_account(char number[]){ //Search Function to get the index of account using the number
    for(int counter=0;counter<Number_of_accounts;counter++){ //Sequential search
        if(strcmp(number,Accounts[counter].Card_number)==0){
            return counter;
        }
    }
    return -1;
}

//CHECK IF NUMBER EXIST FUNCTION
bool ATM:: found(char number[]){ //Search Function to make sure that an account exists
    for(int counter=0;counter<Number_of_accounts;counter++){ //Sequential search
        if(strcmp(number,Accounts[counter].Card_number)==0){
            return 1;
        }
    }
    return 0;
}

//ACCOUNT CREATION FUNCTION
void ATM::create_new_acc(){ //Creates a new account

            //NAME INPUT
            cout<<"Please enter the account name: ";
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
            repeat_name: //Label to repeat the input process
            cin.getline(Account_name,101);
            if(!cin){ //Checking failure of cin (long input for example)
                cout<<"Please, enter a name of 100 letters max: ";
                cin.clear(); //Exiting cin failure mode
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
                goto repeat_name; //Repeats the process
            }
            if(strlen(Account_name)==0){
                cout<<"Empty name is not accepted. Try again: ";
                goto repeat_name;
            }
            for(int counter=0;counter<strlen(Account_name);counter++){ //Checking if all characters are alphabetic or spaces or NULL terminator only
                if(!((Account_name[counter]>=65 && Account_name[counter]<=90) or (Account_name[counter]>=97 && Account_name[counter]<=122) or Account_name[counter]==32 or Account_name[counter]==0)){
                  cout<<"Please, enter valid characters (only lowercase and uppercase English letters are accepted): ";
                  goto repeat_name;
                }
                if(Account_name[0]==32){ //Checking if name starts with space (forbidden)
                  cout<<"A name cannot start with space, try again: ";
                  goto repeat_name;
                }
                if(Account_name[counter]==32 && Account_name[counter+1]==32){ //Checking if name contains double space (forbidden)
                  cout<<"Double space is not allowed, try again: ";
                  goto repeat_name;
                }
                if(Account_name[counter]== Account_name[counter+1] &&Account_name[counter+1]== Account_name[counter+2]&&Account_name[counter+2]== Account_name[counter+3]){ //Checking if name contains repeated character for 4 times (forbidden)
                  cout<<"Repeated characters are not accepted (Unrealistic name). Try again: ";
                  goto repeat_name;
                }
            }

            //NUMBER INPUT
            cout<<"Please enter the account number (10 numbers): ";
            repeat_number: //Label to repeat the input process
            cin.getline(Card_number,11);
            while(!validate_number(Card_number)){ //Calling the validate function
            cin.getline(Card_number,11);
            }
            if(found(Card_number)){ //Checking if not unique
                cout<<"Please, use a unique number: this number is used by another account: ";
                goto repeat_number; //repeats the input process
            }
            int repeated=0;
            int consecutive=0;
            for(int counter=0;counter<10;counter++){
                if(Card_number[counter]==Card_number[0]){ //Checking if a number is a sequence of single repeated number (forbidden because easy to guess)
                    repeated++;
                    if(repeated==10){
                        cout<<"Easy to guess: a sequence of repeated number detected. Try again: ";
                        goto repeat_number; //repeats the input process
                    }
                }
                if(Card_number[counter]==Card_number[counter-1]+1){ //Checking if a number is a sequence of consecutive numbers (forbidden because easy to guess)
                    consecutive++;
                    if(consecutive==8){
                        cout<<"Easy to guess: consecutive numbers detected. Try again: ";
                        goto repeat_number; //repeats the input process
                    }
                }
                else consecutive=0;
            }
            for(int counter=0;counter<10;counter++){ //Checking if a pattern of numbers is used (considered as easy to guess number)
                for(int subcounter=2;subcounter<=5;subcounter++){
                    char pattern[6]={0};
                    for(int subsubcounter=0;subsubcounter<subcounter;subsubcounter++){
                        pattern[subsubcounter]=Card_number[subsubcounter+counter]; //Creating a pattern
                    }
                    int newstart=0; int start=0; int second=0;
                    for(int newcounter=0; newcounter<10;newcounter++){ //Checking for occurrence
                        newstart++;
                        char checkpattern[6]={0};
                        for(int coco=0;coco<subcounter;coco++){
                            checkpattern[coco]=Card_number[coco+newcounter];
                        }
                        if(strcmp(pattern,checkpattern)==0){
                            if(start==0){start++; newstart=0;} //Pattern detected for the first time
                            if(subcounter==2){ //Pattern of 2 numbers detected for the second time
                                if(newstart==subcounter) second=1;
                                if(newstart==subcounter*2 && second==1){ //Pattern of 2 numbers detected for the third time
                                    cout<<"Easy to guess: pattern of "<<pattern<<" detected. Try again: ";
                                    goto repeat_number;
                                }
                            }
                            else{
                                if(newstart==subcounter){ //Pattern of 3 or 4 or 5 numbers detected for the second time
                                    cout<<"Easy to guess: pattern of "<<pattern<<" detected. Try again: ";
                                    goto repeat_number;
                                }
                            }

                        }
                    }
                }
            }
}

//TRANSFER FUNCTION
void ATM::Transfer(int rec,float trans){ //Function to transfer money (receiver index,money to transfer)
    Amount-=1.015*trans;
    Accounts[rec].Amount+=0.985*trans;
    cout<<"sending account:"<<endl;
    Print_acc_data(); //Printing account data of sender
    cout<<"receiving account:"<<endl;
    Accounts[rec].Print_acc_data(); //Printing account data
}

//DEPOSIT FUNCTION
void ATM::Deposit(float deposit_amount){
    Amount+=1.01*deposit_amount;
    Print_acc_data(); //Printing account data
}

//PRINT DATA FUNCTION
void ATM::Print_acc_data(){ //Prints account data of any account
    cout<<"name: "<<Account_name<<endl<<"Card number: "<<Card_number<<endl<<"amount: "<<Amount<<endl;
}

//WITHDRAW FUNCTION
void ATM::Withdrawal(float with_amount){ //Function to withdraw money
    Amount-=1.03*with_amount;
    Print_acc_data();
}

//MAIN NAVIGATION MENU
int Exit=0; //Exit condition is 1 to exit the whole program
void display_navigation_menu(){ //Main navigation menu of the banking system
    cout<<"1----Create new account"<<endl<<"2----Deposit"<<endl<<"3----transfer to another account" <<endl<< "4----withdrawal"<< endl<< "5----exit"<<endl<<"Please enter the number of operation: ";

    //USER OPERATION INPUT
    int required_operation; //Stores the input number
    repeat_operation: //Label to repeat the operation
    cin>>required_operation;
    while(!cin or cin.peek()!='\n'){ //Checks failure (wrong types) or extra characters
              cout<<"INCORRECT INPUT. Please, choose number from 1 to 5: ";
              cin.clear(); //Exiting cin failure mode
              cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
              goto repeat_operation; //repeats the process
    }

    //ALL PROGRAM AVAILABLE OPERATIONS
    switch(required_operation){

    //ACCOUNT CREATION
    case 1:{
        if(Number_of_accounts==100) {cout<<"Max number of accounts has been reached (100)"; break;} //In case max is reached
        Accounts[Number_of_accounts].create_new_acc(); //Creating the new account of number n
        Accounts[Number_of_accounts].Print_acc_data(); //Prints all information about the created account
        Number_of_accounts++; //Increment
        break; //repeats the process
        }

    //DEPOSIT OPERATION
    case 2:{
        if(Number_of_accounts<1){ //Make sure at least one account is active for deposit operation
            cout<<"There're no active bank accounts to do such operations: at least 1 account required."<<endl;
            break;
        }
        cout<<"Please enter the account number(10 numbers): ";
        char deposit_number[11];
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
        repeat_deposit_number: //Label to repeat the process
        cin.getline(deposit_number,11);
        while(!validate_number(deposit_number)){ //Calling the validate function
        cin.getline(deposit_number,11); //Repeats the process
        }
        if(Accounts[0].found(deposit_number)==0){ //Calling the found function to check if a number exists
            cout<<"Account was not found, try again: ";
            goto repeat_deposit_number;
        }
        int index_of_deposit=Accounts[0].index_of_account(deposit_number); //Calling the function to find the index of account using the number
        cout<<"Please enter the amount to be deposited: ";
        float amount_of_deposit;
        repeat_amount_of_deposit: //Label to repeat the process
        cin>> amount_of_deposit;
        while(!cin or cin.peek()!='\n'){ //Checks failure (wrong types) or extra characters
            cout<<"INCORRECT INPUT. Please enter a positive number: ";
            cin.clear(); //Exiting cin failure mode
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
            goto repeat_amount_of_deposit;
        }
        while(amount_of_deposit<=0){ //Checking that the amount is positive number
            cout<<"INCORRECT INPUT. Please enter a positive number: ";
            goto repeat_amount_of_deposit;
        }
        Accounts[index_of_deposit].Deposit(amount_of_deposit); //Calling the deposit function
        break;}

    //TRANSFER OPERATION
    case 3:{
        if(Number_of_accounts<2){ //Make sure at least 2 accounts are active for transfer operation
            cout<<"There're no active bank accounts to do such operations: at least 2 accounts required."<<endl;
            break;
        }
        if(!makesure()){ //Calling the function to make sure that at least one account is valid for withdrawing or transfering
            cout<<"There're no active bank accounts with sufficient money to do such operations: please, deposit first."<<endl;
            break;
        }
        cout<<"please enter the number of the sending account(10 numbers): ";
        char send_number[11];
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
        repeat_send_number: //Label to repeat the process
        cin.getline(send_number,11);
        while(!validate_number(send_number)){ //Calling the validate function
        cin.getline(send_number,11); //Repeats the process
        }
        if(Accounts[0].found(send_number)==0){ //Calling the found function to make sure an account exists
            cout<<"Account was not found, try again: ";
            goto repeat_send_number; //Repeating the process
        }
        int index_of_send=Accounts[0].index_of_account(send_number); //Getting the index of sender by number
        if(Accounts[index_of_send].get_amount()==0){ //Making sure an account has any money
            cout<<"Account has no money, choose another account: ";
            goto repeat_send_number; //Repeating the process
        }
        cout<<"please enter the number of the receiving account(10 numbers): ";
        char rec_number[11];
        repeat_rec_number: //Label to repeat the process
        cin.getline(rec_number,11);
        while(!validate_number(rec_number)){//Calling the validate function
        cin.getline(rec_number,11);//Repeats the process
        }
        if(Accounts[0].found(rec_number)==0){ //Calling the found function to make sure an account exists
            cout<<"Account was not found, try again: ";
            goto repeat_rec_number; //Repeating the process
        }
        if(strcmp(rec_number,send_number)==0){ //Making sure that transfer is not to the same account
            cout<<"Cannot transfer to same account, choose another account: ";
            goto repeat_rec_number;  //Repeating the process
        }
        int index_of_rec=Accounts[0].index_of_account(rec_number); //Getting the index of receiver by number
        cout<<"please enter the amount to be transferred: ";
        float amount_of_trans;
        repeat_amount_of_trans: //Label to repeat the process
        cin>> amount_of_trans;
        while(!cin or cin.peek()!='\n'){ //Checks failure (wrong types) or extra characters
            cout<<"INCORRECT INPUT. Please enter a positive number: ";
            cin.clear(); //Exiting cin failure mode
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
            goto repeat_amount_of_trans;
        }
        while(amount_of_trans<=0){ //Checking amount is positive
            cout<<"INCORRECT INPUT. Please enter a positive number: ";
            goto repeat_amount_of_trans; //Repeating the process
        }
        if(amount_of_trans*1.015>Accounts[index_of_send].get_amount()){ //Checking sender balance
            cout<<"Cannot transfer this amount, account has insufficient balance, try again: ";
            goto repeat_amount_of_trans; //Repeating the process
        }
        Accounts[index_of_send].Transfer(index_of_rec,amount_of_trans); //Calling the transfer function
        break;}

    //WITHDRAWAL OPERATION
    case 4:{
        if(Number_of_accounts<1){ //Make sure at least one account is active for withhdrawal operation
            cout<<"There're no active bank accounts to do such operations: at least 1 account required."<<endl;
            break;
        }
        if(!makesure()){ //Calling the function to make sure that at least one account is valid for withdrawing or transfering
            cout<<"There're no active bank accounts with sufficient money to do such operations: please, deposit first."<<endl;
            break;
        }
        cout<<"Please enter the account number(10 numbers): ";
        char with_number[11];
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
        repeat_with_number: //Label to repeat the process
        cin.getline(with_number,11);
        while(!validate_number(with_number)){ //Calling the validate function
        cin.getline(with_number,11); //Repeating the process
        }
        if(Accounts[0].found(with_number)==0){ //Checking that a number exists
            cout<<"Account was not found, try again: ";
            goto repeat_with_number; //Repeating the process
        }
        int index_of_with=Accounts[0].index_of_account(with_number); //Getting the index of the account
        if(Accounts[index_of_with].get_amount()==0){ //Checking that it has money
            cout<<"Account has no money, choose another account: ";
            goto repeat_with_number; //Repeating the process
        }
        cout<<"Please enter the amount to be withdrawn: ";
        float amount_of_with=0;
        repeat_amount_of_with: //Label to repeat the process
        cin>> amount_of_with;
        while(!cin or cin.peek()!='\n'){ //Checks failure (wrong types) or extra characters
            cout<<"INCORRECT INPUT. Please enter a positive number: ";
            cin.clear(); //Exiting cin failure mode
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Cleaning the buffer
            goto repeat_amount_of_with; //Repeating the process
        }
        while(amount_of_with<=0){ //Checking that the amount is positive
            cout<<"INCORRECT INPUT. Please enter a positive number: ";
            goto repeat_amount_of_with; //Repeating the process
        }
        if(amount_of_with*1.03>Accounts[index_of_with].get_amount()){ //Checking balance
            cout<<"Cannot withdraw this amount, account has insufficient balance, try again: ";
            goto repeat_amount_of_with; //Repeating the process
        }
        Accounts[index_of_with].Withdrawal(amount_of_with); //Calling the withdrawal function
        break;}

    //EXIT OPERATION
    case 5:
        cout << "Thank you for using our ATM system."<<endl;
        Exit=1; //Exit state is active
        break;

    //WRONG NUMBER ENTERED
    default:
        cout<<"INCORRECT INPUT. Please, choose number from 1 to 5: "<<endl;
        return;
    }
}

//MAIN FUNCTION
int main()
{
    display_navigation_menu(); //Displaying the main navigation menu
    while(Exit==0){ //Checking if in exit state or not yet
        display_navigation_menu();
    }
    return 0;
}
