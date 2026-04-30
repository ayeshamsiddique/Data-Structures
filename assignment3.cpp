#include <iostream>
#include<string>
using namespace std;

enum relationType { LESS, GREATER };

class itemType
{
private:
    int flightID;
    string destination;
    string departureTime;
    double flightDuration;
    string flightStatus;

public:
    itemType()
    {
        flightID = 0;
        destination = "";
        departureTime = "";
        flightDuration = 0.0;
        flightStatus = "";
    }

    itemType(int id, string dest, string depTime, double duration, string status)
    {
        flightID = id;
        destination = dest;
        departureTime = depTime;
        flightDuration = duration;
        flightStatus = status;
    }

    void setFlightID(int id)
    {
        flightID = id;
    }

    void setDestination(string dest)
    {
        destination = dest;
    }

    void setDepartureTime(string time)
    {
        departureTime = time;
    }

    void setFlightDuration(double duration)
    {
        flightDuration = duration;
    }

    void setFlightStatus(string status)
    {
        flightStatus = status;
    }

    int getFlightID()
    {
        return flightID;
    }

    string getDestination()
    {
        return destination;
    }

    string getDepartureTime()
    {
        return departureTime;
    }

    double getFlightDuration()
    {
        return flightDuration;
    }

    string getFlightStatus()
    {
        return flightStatus;
    }

    relationType compareTo(const itemType& other) const
    {
        if (flightID <= other.flightID)
            return GREATER;
        else
            return LESS;
    }

    void display()
    {
        cout << "Flight ID: " << flightID << endl;
        cout << "Destination: " << destination << endl;
        cout << "Departure Time: " << departureTime << endl;
        cout << "Flight Duration: " << flightDuration << " hours" << endl;
        cout << "Flight Status: " << flightStatus << endl;
    }
};

class node
{
public:
    node* parent;
    node* left;
    node* right;
    itemType key;

    node(itemType k)
    {
        key = k;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }
};

class BST
{
private:
    node* root;

    node* minimum(node* x)
    {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    void transplant(node* u, node* v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    void inorder(node* x)
    {
        if (x != nullptr)
        {
            inorder(x->left);
            x->key.display();
            cout << endl;
            inorder(x->right);
        }
    }

    void inorderByDestination(node* x, string dest)
    {
        if (x != nullptr)
        {
            inorderByDestination(x->left, dest);

            string flightDest = toLowerCase(x->key.getDestination());
            string searchDest = toLowerCase(dest);

            if (flightDest == searchDest)
            {
                x->key.display();
                cout << endl;
            }
            inorderByDestination(x->right, dest);
        }
    }

    string toLowerCase(string str)
    {
        string result = "";
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] >= 'A' && str[i] <= 'Z')
                result = result + char(str[i] + 32);
            else
                result = result + str[i];
        }
        return result;
    }

    bool flightIDExists(node* x, int id)
    {
        if (x == nullptr)
            return false;
        if (x->key.getFlightID() == id)
            return true;
        if (id < x->key.getFlightID())
            return flightIDExists(x->left, id);
        else
            return flightIDExists(x->right, id);
    }

    bool isValidTime(string time)
    {
        if (time.length() != 5)
            return false;

        if (time[2] != ':')
            return false;

        for (int i = 0; i < time.length(); i++)
        {
            if (i == 2)
                continue;
            if (time[i] < '0' || time[i] > '9')
                return false;
        }

        int hours = (time[0] - '0') * 10 + (time[1] - '0');
        int minutes = (time[3] - '0') * 10 + (time[4] - '0');

        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
            return false;

        return true;
    }

    bool isValidStatus(string status)
    {
        string lowerStatus = toLowerCase(status);
        return (lowerStatus == "departed" || lowerStatus == "landed" || lowerStatus == "delayed");
    }

    void inorderByStatus(node* x, string status)
    {
        if (x != nullptr)
        {
            inorderByStatus(x->left, status);

            string flightStatus = toLowerCase(x->key.getFlightStatus());
            string searchStatus = toLowerCase(status);

            if (flightStatus == searchStatus)
            {
                x->key.display();
                cout << endl;
            }

            inorderByStatus(x->right, status);
        }
    }

    void inorderETA(node* x)
    {
        if (x != nullptr)
        {
            inorderETA(x->left);

            cout << "Flight ID: " << x->key.getFlightID() << endl;
            cout << "Destination: " << x->key.getDestination() << endl;
            cout << "Departure Time: " << x->key.getDepartureTime() << endl;
            cout << "Flight Duration: " << x->key.getFlightDuration() << " hours" << endl;

            // Simple ETA calculation
            string depTime = x->key.getDepartureTime();
            double duration = x->key.getFlightDuration();

            // Extract hours and minutes from departure time
            int hours = 0;
            int minutes = 0;

            // Find the colon position
            int colonPos = 0;
            for (int i = 0; i < depTime.length(); i++)
            {
                if (depTime[i] == ':')
                {
                    colonPos = i;
                    break;
                }
            }

            // Convert string to integer
            string hourStr = "";
            string minStr = "";

            for (int i = 0; i < colonPos; i++)
                hourStr = hourStr + depTime[i];

            for (int i = colonPos + 1; i < depTime.length(); i++)
                minStr = minStr + depTime[i];

            hours = 0;
            for (int i = 0; i < hourStr.length(); i++)
                hours = hours * 10 + (hourStr[i] - '0');

            minutes = 0;
            for (int i = 0; i < minStr.length(); i++)
                minutes = minutes * 10 + (minStr[i] - '0');

            // Add duration to departure time
            int totalMinutes = hours * 60 + minutes + (duration * 60);
            int etaHours = (totalMinutes / 60) % 24;
            int etaMinutes = totalMinutes % 60;

            cout << "Expected Time of Arrival (ETA): ";
            if (etaHours < 10)
                cout << "0";
            cout << etaHours << ":";
            if (etaMinutes < 10)
                cout << "0";
            cout << etaMinutes << endl;

            string punctuality = "On-Time";
            string status = toLowerCase(x->key.getFlightStatus());

            if (status == "delayed")
                punctuality = "Delayed";
            else if (status == "landed")
                punctuality = "Early";
            else if (status == "departed")
                punctuality = "On-Time";

            cout << "Punctuality Status: " << punctuality << endl;
            cout << endl;

            inorderETA(x->right);
        }
    }

public:
    BST()
    {
        root = nullptr;
    }

    void Insert(node* z)
    {
        node* y = nullptr;
        node* x = root;
        while (x != nullptr)
        {
            y = x;
            if (z->key.getFlightID() < x->key.getFlightID())
                x = x->left;
            else
                x = x->right;
        }
        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (z->key.getFlightID() < y->key.getFlightID())
            y->left = z;
        else
            y->right = z;
    }

    void Delete(node* z)
    {
        if (z->left == nullptr)
            transplant(z, z->right);
        else if (z->right == nullptr)
            transplant(z, z->left);
        else
        {
            node* y = minimum(z->right);
            if (y->parent != z)
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
        }
        delete z;
    }

    node* search(node* x, int flightID)
    {
        if (x == nullptr || flightID == x->key.getFlightID())
            return x;
        if (flightID < x->key.getFlightID())
            return search(x->left, flightID);
        else
            return search(x->right, flightID);
    }

    bool checkFlightIDExists(int id)
    {
        return flightIDExists(root, id);
    }

    bool validateTime(string time)
    {
        return isValidTime(time);
    }

    bool validateStatus(string status)
    {
        return isValidStatus(status);
    }

    node* getRoot()
    {
        return root;
    }

    void display()
    {
        cout << "All Flights (Sorted by Flight ID)" << endl;
        if (root == nullptr)
        {
            cout << "No flights available." << endl;
        }
        else
        {
            inorder(root);
        }
    }

    void collectDestinations(node* x, string destinations[], int& count)
    {
        if (x != nullptr)
        {
            collectDestinations(x->left, destinations, count);

            // Check if destination already exists
            bool found = false;
            string currentDest = toLowerCase(x->key.getDestination());

            for (int i = 0; i < count; i++)
            {
                if (toLowerCase(destinations[i]) == currentDest)
                {
                    found = true;
                    break;
                }
            }

            // Add new destination
            if (!found)
            {
                destinations[count] = x->key.getDestination();
                count++;
            }

            collectDestinations(x->right, destinations, count);
        }
    }

    void displayFlightsByDestination()
    {
        cout << "--Flights Grouped by Destination--" << endl;

        if (root == nullptr)
        {
            cout << "No flights available." << endl;
            return;
        }

        // Collect all unique destinations
        string destinations[10];
        int destCount = 0;
        collectDestinations(root, destinations, destCount);

        // Display flights for each destination
        for (int i = 0; i < destCount; i++)
        {
            cout << "Flights to " << destinations[i] << endl;
            inorderByDestination(root, destinations[i]);
        }
    }

    void displayFlightsByStatus()
    {
        cout << "--Flights Grouped by Status--" << endl;

        if (root == nullptr)
        {
            cout << "No flights available." << endl;
            return;
        }

        cout << "--Departed Flights--" << endl;
        inorderByStatus(root, "Departed");

        cout << "--Landed Flights--" << endl;
        inorderByStatus(root, "Landed");

        cout << "--Delayed Flights--" << endl;
        inorderByStatus(root, "Delayed");
    }

    void displayETAForAllFlights()
    {
        cout << "--ETA and Punctuality Status for All Flights--" << endl;

        if (root == nullptr)
        {
            cout << "No flights available" << endl;
        }
        else
        {
            inorderETA(root);
        }
    }

    // Surprise feature to show total count
    int countFlights(node* node) 
    {
        if (node == nullptr)
            return 0;
        return 1 + countFlights(node->left) + countFlights(node->right);
    }

    void displayTotalFlights()
    {
        cout << "Total number of flights: " << countFlights(root) << endl;
    }
};

int main()
{
    BST flightTree;
    int choice;

    do
    {
        cout << "FLIGHT RESERVATION DIRECTORY" << endl;
        cout << "1. Insert new flight record" << endl;
        cout << "2. Delete a flight record" << endl;
        cout << "3. Search for a flight" << endl;
        cout << "4. Display all flights (sorted by Flight ID)" << endl;
        cout << "5. Display flights grouped by destination" << endl;
        cout << "6. Display flights grouped by status" << endl;
        cout << "7. Display ETA and punctuality status for all flights" << endl;
        cout << "8. Display total number of flights" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        // Check if input failed (non-numeric input)
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "enter a valid number (1-9)" << endl;
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int id;
            string dest, depTime, status;
            double duration;
            bool validInput = false;

            // Input Flight ID
            while (!validInput)
            {
                cout << "Enter Flight ID: ";
                cin >> id;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Flight ID must be a number" << endl;
                    continue;
                }

                // Validate Flight ID is positive
                if (id <= 0)
                {
                    cout << "Flight ID must be a positive number" << endl;
                    continue;
                }

                // Check if Flight ID already exists
                if (flightTree.checkFlightIDExists(id))
                {
                    cout << "Flight ID " << id << " already exists" << endl;
                    continue;
                }
                validInput = true;
            }

            cin.ignore();

            // Input Destination with validation loop
            validInput = false;
            while (!validInput)
            {
                cout << "Enter Destination: ";
                getline(cin, dest);

                // Validate destination is not empty
                if (dest.empty())
                {
                    cout << "Destination cannot be empty" << endl;
                    continue;
                }
                validInput = true;
            }

            // Input Departure Time with validation loop
            validInput = false;
            while (!validInput)
            {
                cout << "Enter Departure Time (HH:MM): ";
                getline(cin, depTime);

                // Validate time format
                if (!flightTree.validateTime(depTime))
                {
                    cout << "Invalid time format. use HH:MM (e.g., 09:30)" << endl;
                    cout << "Hours must be 00-23 and minutes must be 00-59" << endl;
                    continue;
                }
                validInput = true;
            }

            // Input Flight Duration with validation loop
            validInput = false;
            while (!validInput)
            {
                cout << "Enter Flight Duration (in hours): ";
                cin >> duration;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Flight duration must be a number" << endl;
                    continue;
                }

                // Validate duration is positive
                if (duration <= 0)
                {
                    cout << "Error: Flight duration must be greater than 0!" << endl;
                    continue;
                }

                // Validate duration is less than 24 hours
                if (duration > 24)
                {
                    cout << "Flight duration cannot exceed 24 hours" << endl;
                    continue;
                }
                validInput = true;
            }
            cin.ignore();

            // Input Flight Status with validation loop
            validInput = false;
            while (!validInput)
            {
                cout << "Enter Flight Status (Departed/Landed/Delayed): ";
                getline(cin, status);

                // Validate status
                if (!flightTree.validateStatus(status))
                {
                    cout << "Invalid status. Use Departed, Landed, or Delayed" << endl;
                    continue;
                }
                validInput = true;
            }

            itemType flight(id, dest, depTime, duration, status);
            node* newNode = new node(flight);
            flightTree.Insert(newNode);
            cout << "Flight record inserted successfully" << endl;
            break;
        }
        case 2:
        {
            int id;
            bool validInput = false;

            // Input Flight ID with validation loop
            while (!validInput)
            {
                cout << "Enter Flight ID to delete: ";
                cin >> id;

                // Check if input failed
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Flight ID must be a number" << endl;
                    continue;
                }

                // Validate Flight ID is positive
                if (id <= 0)
                {
                    cout << "Flight ID must be a positive number" << endl;
                    continue;
                }
                validInput = true;
            }

            node* foundNode = flightTree.search(flightTree.getRoot(), id);
            if (foundNode != nullptr)
            {
                flightTree.Delete(foundNode);
                cout << "Flight record deleted successfully" << endl;
            }
            else
            {
                cout << "Flight not found" << endl;
            }
            break;
        }
        case 3:
        {
            int id;
            bool validInput = false;

            // Input Flight ID with validation loop
            while (!validInput)
            {
                cout << "Enter Flight ID to search: ";
                cin >> id;

                // Check if input failed
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Flight ID must be a number" << endl;
                    continue;
                }

                // Validate Flight ID is positive
                if (id <= 0)
                {
                    cout << "Flight ID must be a positive number" << endl;
                    continue;
                }
                validInput = true;
            }

            node* foundNode = flightTree.search(flightTree.getRoot(), id);
            if (foundNode != nullptr)
            {
                cout << "--Flight Details--" << endl;
                foundNode->key.display();
            }
            else
            {
                cout << "Flight not found" << endl;
            }
            break;
        }
        case 4:
        {
            flightTree.display();
            break;
        }
        case 5:
        {
            flightTree.displayFlightsByDestination();
            break;
        }
        case 6:
        {
            flightTree.displayFlightsByStatus();
            break;
        }
        case 7:
        {
            flightTree.displayETAForAllFlights();
            break;
        }
        case 8:
        {
            flightTree.displayTotalFlights();
            break;
        }
        case 9:
        {
            cout << "Exiting program" << endl;
            break;
        }
        default:
        {
            cout << "Invalid choice! try again" << endl;
            break;
        }
        }
    } while (choice != 9);
    return 0;
}
