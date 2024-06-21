#include <iostream>
#include <ctime>            //converts the time value pointed to by time to local time in the form of a character string
#include <chrono>           //used to deal with date and time
#include <sstream>          //allows you to manipulate strings as if they were input or output streams
#include <string>           //defines one variable type, one macro, and many functions which can be used to manipulate a string or array of characters
#include <vector>
#include <algorithm>
#include <tuple>
using std::tuple;
using namespace std;


pair<int, int> getCurrentTime() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* timeInfo = localtime(&currentTime);

    int hour = timeInfo->tm_hour;
    int minute = timeInfo->tm_min;

    return make_pair(hour, minute);
}

void PrintDay(int dayIndex)
{
    int day = dayIndex % 7;
    cout << "Tasks for ";
    switch (day) {
        case 0:
            cout << "Monday";
            break;
        case 1:
            cout << "Tuesday";
            break;
        case 2:
            cout << "Wednesday";
            break;
        case 3:
            cout << "Thursday";
            break;
        case 4:
            cout << "Friday";
            break;
        case 5:
            cout << "Saturday";
            break;
        case 6:
            cout << "Sunday";
            break;
        default:
            cout << "Invalid day index";
            break;
    }
    cout << ":\n";
}


class Node
{
    public :
    string type;
    string task;
    int n;
    string *subt;
    int s_hr;
    int s_min;
    int e_hr;
    int e_min;

    Node *next;

    Node(string t,string tk,int num,string sub[],int sh,int sm,int eh,int em) : type(t), task(tk),n(num), subt(sub), s_hr(sh), s_min(sm), e_hr(eh), e_min(em), next(NULL){}

    ~Node()
    {
        delete[] subt;
    }
    
};

class Week
{
    Node* days[7];

    public :

    Week()
    {
        for (int i = 0; i < 7; ++i) {
            days[i] = NULL;
        }
    }


    void Add(int dayno)
    {
        string type;
        cout << "Is it Work / Personal (W/P) : ";
        cin >> type;

        string task;
        cout << "Your Task : ";
        cin.ignore();
        getline(cin, task);

        int n;
        cout << "No. of SubTask : ";
        cin >> n;
        cin.ignore();
        string *sub = new string[n];
        for (int i = 0; i < n; i++)
        {
            cout << i + 1 << ". -> ";
            getline(cin, sub[i]);
        }

        string shm;
        cout << "Start Hour/Min (hh:mm) : ";
        cin >> shm;

        istringstream ss1(shm);
        string hour_str, min_str;
        getline(ss1, hour_str, ':');
        getline(ss1, min_str, ':');

        int s_hr = stoi(hour_str);
        int s_min = stoi(min_str);


        string ehm;
        cout << "End Hour/Min (hh:mm) : ";
        cin >> ehm;

        istringstream ss2(ehm);
        getline(ss2, hour_str, ':');
        getline(ss2, min_str, ':');

        int e_hr = stoi(hour_str);
        int e_min = stoi(min_str);


        Node *temp = new Node(type, task, n, sub, s_hr, s_min, e_hr, e_min);

        if (days[dayno] != NULL)
        {
            Node *curr = days[dayno];
            Node *prev = NULL;

            while (curr && days[dayno] && (s_hr > days[dayno]->s_hr || (s_hr == days[dayno]->s_hr && s_min > days[dayno]->s_min)))
            {
                prev = curr;
                curr = curr->next;
            }

            if (prev)
            {
                prev->next = temp;
                temp->next = curr;
            }
            else
            {
                temp->next = days[dayno];
                days[dayno] = temp;
            }
        }
        else
        {
            days[dayno] = temp;
        }
    }


    void Remove(int dayno,int idx=1)
    {
        if(!days[dayno])
            return;

        Node *temp;
            
        if (idx == 1)
        {            
            temp = days[dayno];
            days[dayno] = days[dayno]->next;
        }
        else
        {
            Node *p = days[dayno];

            for (int i = 1; i < idx - 1 && p; i++)
            {
                p = p->next;
            }

            if (p)
            {
                temp = p->next;
                p->next = temp->next;
            }
        }
        delete temp;
        
    }

    void Display(int dayIndex) {
        if (dayIndex >= 0 && dayIndex < 7) {
            Node* day = days[dayIndex];
            PrintDay(dayIndex);

            Node* p = day;
            while (p) {
                cout << "Start Time: " << p->s_hr << ":" << p->s_min << endl;
                cout << "End Time: " << p->e_hr << ":" << p->e_min << endl;
                cout << "Task: " << p->task << endl;
                cout << "Type: " ;
                if(p->type=="W" || p->type=="w")
                    cout<<"Work "<<endl;
                else if (p->type=="P" || p->type=="p")
                    cout<<"Personal "<<endl;
                else
                    cout<<p->type<<endl;

                // Display subtasks if available
                if (p->subt != NULL) 
                {                    
                    cout << "Subtasks:\n";
                    for (int i = 0; i < p->n; ++i)
                    {
                        cout << i + 1 << ". -> " << p->subt[i] << endl;
                    }
                }
                cout << "-----------------------------\n";
                p = p->next;
            }
        } else 
        {
            cout << "Invalid day index." << endl;
        }
    }

    void Upcoming(int dayIndex)
    {
        if (dayIndex >= 0 && dayIndex < 7)
            PrintDay(dayIndex);
        
        Node* day = days[dayIndex];
        std::pair<int, int> currentTime = getCurrentTime();

        int hour = currentTime.first;
        int minute = currentTime.second;

        Node *p = day;
        while(p && (hour > p->s_hr || (hour==p->s_hr && minute > p->s_min)))
        {
            p=p->next;
        }

        if(p)
        {
            cout << "-----------------------------\n";
            cout << "Start Time: " << p->s_hr << ":" << p->s_min << endl;
            cout << "End Time: " << p->e_hr << ":" << p->e_min << endl;
            cout << "Task: " << p->task << endl;
            cout << "Type: " ;
            if(p->type=="W" || p->type=="w")
                cout<<"Work "<<endl;
            else if (p->type=="P" || p->type=="p")
                cout<<"Personal "<<endl;
            else
                cout<<p->type<<endl;

            // Display subtasks if available
            if (p->subt != NULL) {
                
                cout << "Subtasks:\n";
                for (int i = 0; i < p->n; ++i) { // Use p->n for the number of subtasks
                    cout << i + 1 << ". -> " << p->subt[i] << endl;
                }
            }

            cout << "-----------------------------\n";
        }
        else
        {
            cout<<"There is no Task left ."<<endl;
        }
    }

    struct Interval 
    {
        int startHour;
        int startMinute;
        int endHour;
        int endMinute;
        string type;
    };

    void PrivacyView(int dayIndex) {

        if (dayIndex < 0 || dayIndex >= 7) {
            cout << "Invalid day index" << endl;
            return;
        }

        PrintDay(dayIndex);

        vector<Interval> intervals;

        Node* day = days[dayIndex];
        
        while (day != NULL) {
            Interval interval;
            interval.startHour = day->s_hr;
            interval.startMinute = day->s_min;
            interval.endHour = day->e_hr;
            interval.endMinute = day->e_min;
            interval.type = (day->type=="W" || day->type=="w") ? ": At Work" : ": Personal Activity";

            intervals.push_back(interval);
            
            day = day->next;
        }

        sort(intervals.begin(), intervals.end(),[](Interval a, Interval b)
        {
             return tie(a.startHour, a.startMinute) < tie(b.startHour, b.startMinute);
        });

        int currentHour = 0, currentMinute = 0;

        for (Interval interval : intervals)
        {

            if (currentHour != interval.startHour || currentMinute != interval.startMinute)
            {
                
                cout << (currentHour < 10 ? "0" : "") << currentHour << ":"  
                    << (currentMinute < 10 ? "0" : "") << currentMinute << " - "
                    << (interval.startHour < 10 ? "0" : "") << interval.startHour << ":"
                    << (interval.startMinute < 10 ? "0" : "") << interval.startMinute
                    << " (Available)" << endl;   
            }

            cout << (interval.startHour < 10 ? "0" : "") << interval.startHour << ":"
                << (interval.startMinute < 10 ? "0" : "") << interval.startMinute << " - "  
                << (interval.endHour < 10 ? "0" : "") << interval.endHour << ":"
                << (interval.endMinute < 10 ? "0" : "") << interval.endMinute << " "
                << interval.type << endl;

            currentHour = interval.endHour;
            currentMinute = interval.endMinute;
        }

        if (currentHour < 24) {
            cout << (currentHour < 10 ? "0" : "") << currentHour << ":"
                << (currentMinute < 10 ? "0" : "") << currentMinute 
                << " - 24:00 (Available)" << endl;
        }

    }
};


int main() 
{
    Week Thurs;
    int dayIndex = 3; // 3 corresponds to Thursday

    while (true) {
        cout << "MENU:- " << endl;
        cout << "1. Display Schedule" << endl;
        cout << "2. Add new Schedule" << endl;
        cout << "3. Remove Schedule" << endl;
        cout << "4. Upcoming Schedule" << endl;
        cout << "5. View with Privacy" << endl;
        cout << "6. Exit" << endl;

        int choice;
        cout << "Enter command : ";
        cin >> choice;

        switch (choice) {
            case 1:
                Thurs.Display(dayIndex);
                break;
            case 2:
                Thurs.Add(dayIndex);
                break;
            case 3:
                cout << "Task no. to be Removed (by order) : ";
                int idx;
                cin >> idx;
                Thurs.Remove(dayIndex,idx);
                break;
            case 4:
                Thurs.Upcoming(dayIndex);
                break;
            case 5:
                Thurs.PrivacyView(dayIndex);
                break;
            case 6:
                cout << "Exiting program." << endl;
                cout << "\nTHANK YOU FOR USING CALENX " << endl;
                cout << ": )" <<endl;
                return 0;
            default:
                cout << "Invalid command. Please choose a valid option." << endl;
                break;
        }
    }

    return 0;
}

