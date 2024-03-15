

#include <iostream>
#include <fstream>
#include <string>


using namespace std;
const int FIRST_SIZE = 10;


class Person {
public:
    int key = 0;
    string FirstName;
    string SecondName;
    string LastName;
    Person* Next = nullptr;
    Person* Previous = nullptr;
    
    
};

class HashTable {
public:
    Person** table;
    int currentSize;
    int tableSize;

    // Хеш-функция 
    int hash(int key) {
        
        // Возводим число в квадрат
        long long squared = pow(key,2);
        std::string binaryString;
        while (squared > 0) {
            binaryString.insert(binaryString.begin(), '0' + (squared % 2));
            squared /= 2;
        }
        std::string middleBinary = binaryString.substr(binaryString.length() / 2 - 5, 10);
        long long result = std::stoi(middleBinary, nullptr, 2);
        return result % tableSize;
        
    }


    

    // Рехеширование 
    void rehash() {
        int newSize = this->tableSize * 2;
        int oldTableSize = tableSize;
        tableSize = newSize;
        Person** oldTable = table;
        Person** newTable = new Person * [newSize];
        table = newTable;
        for (int i = 0; i < newSize; ++i) {
            newTable[i] = nullptr;
        }

        for (int index = 0; index < oldTableSize; ++index) {
            Person* Element = oldTable[index];
            if (Element != nullptr)
            {
                if (Element->Next == nullptr) 
                {
                    insert(Element->key, Element->FirstName, Element->SecondName, Element->LastName);
                }
                else 
                {
                    while (Element->Next != nullptr)
                    {
                        Element = Element->Next;
                    }
                    while (Element->Previous != nullptr)
                    {
                        insert(Element->key, Element->FirstName, Element->SecondName, Element->LastName);
                        Element = Element->Previous;
                    }                   
                    insert(Element->key, Element->FirstName, Element->SecondName, Element->LastName);
                }

            }
        }
        
    }


    HashTable() : currentSize(0), tableSize(FIRST_SIZE) {
        table = new Person * [tableSize];
        for (int i = 0; i < tableSize; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        // Очистка выделенной памяти
        for (int i = 0; i < tableSize; ++i) {
            delete table[i];
        }
        delete[] table;
    }

    // Вставка записи
    void insert(int key, const string& FirstName, const string& SecondName, const string& LastName) {
        if (currentSize >= tableSize * 0.75) {
            rehash();
        }

        int index = hash(key);
        if (table[index] == nullptr) 
        {
            table[index] = new Person;
            table[index]->key = key;
            table[index]->FirstName = FirstName;
            table[index]->SecondName = SecondName;
            table[index]->LastName = LastName;
        }
        else 
        {
            table[index]->Next = new Person;
            table[index]->Next->FirstName = FirstName;
            table[index]->Next->key = key;
            table[index]->Next->SecondName = SecondName;
            table[index]->Next->LastName = LastName;
            table[index]->Next->Previous = table[index];
            
        }
        currentSize++;
    }

    // Поиск записи по ключу
    Person* search(int key) 
    {
        int index = hash(key);
        if (table[index]->key == key) 
        {
            return table[index];
        }
        else 
        {
            Person* Element = table[index];
            while (Element->key != key) 
            {
                Element = Element->Next;
            }
            return Element;
        }
    }

    // Удаление записи по ключу
    void remove(int key) {
        int index = hash(key);
        if (table[index]->key == key && table[index]->Next == nullptr)
        {
           delete table[index];
           table[index] = nullptr;
        }
        else 
        {
            Person* Element = table[index];
            while (Element->Next != nullptr) 
            {
                if (Element->key == key) 
                {
                    break;
                }
                else 
                {
                    Element = Element->Next;
                }
            }
            if (Element->key == key) 
            {
                Person* PreviousElement = nullptr;
                Person* NextElement = nullptr;
                if (Element->Previous != nullptr) 
                {
                    PreviousElement = Element->Previous;
                }
                else
                {
                    Person* Element = table[index];
                    table[index] = table[index]->Next;
                    delete Element;
                    Element = nullptr;
                    return;
                }

                if (Element->Next != nullptr)
                {
                    NextElement = Element->Next;
                }
                else
                {
                    delete Element;
                    Element = nullptr;
                }
               
                if (PreviousElement != NextElement) 
                {
                    PreviousElement->Next = NextElement;
                    delete Element;
                    Element = nullptr;
                }
         
            }
           
        }
       
       
    }


    void printTable() {
        cout << "=============================================================================================" << endl;
        cout << "Current Table:" << "(Current Size: " << currentSize << ", Table Size: " << tableSize << ")" << endl;
        cout << "=============================================================================================" << endl;

        for (int i = 0; i < tableSize; ++i) {
            Person* Element = table[i];
            if (Element != nullptr) {
                while (Element != nullptr) 
                {
                    cout << "Index " << i << ": Key = " << Element->key << ", FirstName = " << Element->FirstName << ", SecondName = " << Element->SecondName << ", LastName = " << Element->LastName << endl;
                    Element = Element->Next;
                }
                
            }
            
        }
        cout << "==============================================================================================" << endl;
    }

    

    
};


int main()
{
    HashTable hashTable;

    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Failed to open the input file." << endl;
        return 1;
    }

    
    string line;
    
    while (getline(inputFile, line)) {
        int key;
        string FirstName;
        string SecondName;
        string LastName;
        int count = 0;
        string buffer;
        line += ' ';
        for (int i = 0; i < line.length(); i++) 
        {
            if ((line[i] != ' '))
            {
                buffer += line[i];
            }
            else 
            {
                if (count == 0)
                {
                    key = stoi(buffer);
                    buffer.clear();
                    count++;
                }
                else 
                {
                    if (count == 1) 
                    {
                        FirstName = buffer;
                        buffer.clear();
                        count++;
                    }
                    else
                    {
                        if (count == 2)
                        {
                            SecondName = buffer;
                            buffer.clear();
                            count++;
                        }
                        else
                        {
                            if (count == 3)
                            {
                                LastName = buffer;
                                buffer.clear();
                                count++;
                            }
                            
                        }
                    }
                }
                
            }
        }
        
        hashTable.insert(key, FirstName, SecondName, LastName);
    }

    inputFile.close();

    CHOICE:
    hashTable.printTable();
    cout << "If you wanna Find some, type F;" << endl;
    cout << "If you wanna Delete some, type D;" << endl;
    cout << "If you wanna Delete some, type A;" << endl;
    cout << "Choose what you are gonna do (choose wisely): " ;
    char choice;
    int addKey;
    string addFirstName;
    string addSecondName;
    string addLastName;
    cin >> choice;
    if (choice == 'F') { goto FIND; }
    if (choice == 'D') { goto DELETE; }
    if (choice == 'A') { goto ADD; }
    ADD:
    cout << "Enter the Key of element you want to add:" << endl;
    cin >> addKey;
    cout << "Enter the First Name of element you want to add:" << endl; 
    cin >> addFirstName;
    cout << "Enter the Second Name of element you want to add:" << endl;  
    cin >> addSecondName;
    cout << "Enter the Last Name of element you want to add:" << endl;
    cin >> addLastName;
    hashTable.insert(addKey, addFirstName, addSecondName, addLastName);
    system("cls");
    hashTable.printTable();
    cout << "Element succecifully added." << endl;
    cout << "Continue? (y/n)" << endl;
    cin >> choice;
    if (choice == 'y')
    {
        system("cls");
        goto CHOICE;
    }
    if (choice == 'n')
    {
        goto EXIT;
    }

    FIND:
    int searchKey;
    cout << "Enter the key of element you want to find: ";
    cin >> searchKey;

    cout << "Search result for key " << searchKey << ": " << hashTable.search(searchKey)->FirstName << " " << hashTable.search(searchKey)->SecondName << " " << hashTable.search(searchKey)->LastName << endl;
    cout << "Continue? (y/n)" << endl;
    cin >> choice;
    if (choice == 'y') 
    {
        system("cls");
        goto CHOICE;
    }
    if (choice == 'n')
    {
        goto EXIT;
    }

    DELETE:
    int deleteKey;
    cout << "Enter the key you want to delete: ";
    cin >> deleteKey;
    hashTable.remove(deleteKey);
    system("cls");
    hashTable.printTable();
    cout << "Record with key " << deleteKey << " deleted." << endl << endl;
    cout << "Continue? (y/n)" << endl;
    cin >> choice;
    if (choice == 'y')
    {
        system("cls");
        goto CHOICE;
    }
    if (choice == 'n')
    {
        goto EXIT;
    }

    EXIT:
    return 0;
}

