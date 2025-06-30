#include <QCoreApplication>
#include <iostream>
#include <string>
#include <QMap>
#include <QMultiMap>
#include <QFile>
#include <QTextStream>
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QMultiMap <string, int> Storage;
    int k=0;
    do{
        cout<<"press 1 to add element "<<endl;
        cout<<"press 2 to delete element "<<endl;
        cout<<"press 3 to find first element before the entered key "<<endl;
        cout<<"press 4 to copy elements to new map "<<endl;
        cout<<"press 5 to change the value of element "<<endl;
        cout<<"press 6 to show the map(Storage) "<<endl;
        cout<<"press 7 to get data from file"<<endl;
        cout<<"press 8 to exit the programme"<<endl;
        cout<<"chose option you want   : ";
        cin>>k;
        switch(k){
        case 1:{
            string key;
            int value;
            cout<<"enter key(name) :"<<endl;
            cin>>key;
            cout<<"enter value(age) :"<<endl;
            cin>>value;
            Storage.insert(key, value);
            break;
        }
        case 2:{
            cout<<"Storage before delete : "<<endl;
            for (auto i=Storage.begin(); i!=Storage.end(); i++)
            {cout<<"key : "<<i.key()<<" value : "<<i.value()<<endl;}
            string bound1, bound2;
            int d;
            cout<<"enter bound 1"<<endl;
            cin>>bound1;
            cout<<"enter bound 2"<<endl;
            cin>>bound2;
            if (!Storage.contains(bound1) or !Storage.contains(bound2))
            {cout<<"QMap doesn't contain your bounds"<<endl;
            break;}
            cout<<"0 - range not includes bonds"<<endl;
            cout<<"1 - range includes bound 1"<<endl;
            cout<<"2 - range includes bound 2"<<endl;
            cout<<"3 - range includes both bounds"<<endl;
            cin>>d;
            if(d<0 or d>3) {
                cout<<"wrong choice"<<endl;
                break;
            }
            if(d==0){
                auto spot1=Storage.upperBound(bound1);
                auto spot2=Storage.lowerBound(bound2);
                Storage.erase(spot1, spot2);
            }
            if(d==1){
                auto spot1=Storage.lowerBound(bound1);
                auto spot2=Storage.lowerBound(bound2);
                Storage.erase(spot1, spot2);
            }
            if(d==2){
                auto spot1=Storage.upperBound(bound1);
                auto spot2=Storage.upperBound(bound2);
                Storage.erase(spot1, spot2);
            }
            if(d==3){
                auto spot1=Storage.lowerBound(bound1);
                auto spot2=Storage.upperBound(bound2);
                Storage.erase(spot1, spot2);
            }
            cout<<"Storage after delete : "<<endl;
            for (auto i=Storage.begin(); i!=Storage.end(); i++)
            {cout<<"key : "<<i.key()<<" value : "<<i.value()<<endl;}
            break;
        }
        case 3: {
            string bound;
            int d;
            cout<<"enter the bound" <<endl;
            cin>>bound;
            if(!Storage.contains(bound)){
                cout<<"the bounds you entered don't exist"<< endl;
                break;
            }
            cout<<"press 1 if you want to find elements lower(including) key"<<endl;
            cout<<"press 2 if you want to find elements lower(excluding) key"<<endl;
            cin>>d;
            if(d!=1 and d!=2) {cout<<"your choice is wrong"<<endl; break;}
            if(d==1){
                auto i=Storage.upperBound(bound);
                for( auto it=Storage.begin(); it!=i; it++)
                    cout<<"key: "<<it.key()<<" value : "<<it.value()<<endl;
                break;
            }
            if(d==2){
                auto i=Storage.lowerBound(bound);
                for( auto it=Storage.begin(); it!=i; it++)
                    cout<<"key: "<<it.key()<<" value : "<<it.value()<<endl;
                break;
            }
            break;
        }
        case 4:{
            QMap<string, int> newstorage;
            cout<<" Storage(map) : "<<endl;
            for (auto i=Storage.begin(); i!=Storage.end(); i++)
            {cout<<"key : "<<i.key()<<" value : "<<i.value()<<endl;}
            for (auto i=Storage.begin(); i!=Storage.end();i++)
            {newstorage.insert(i.key(), i.value());}
            QFile file("C:/Users/HP/Documents/laba7/newstorage.txt");
            if(!file.open(QIODevice::WriteOnly)){
                cout<<"eror in file opening"<<endl;
                break;
            }
            QTextStream out(&file);
            cout<<" NewStorage(multimap) : "<<endl;
            for (auto i=newstorage.begin(); i!=newstorage.end(); i++){
                cout<<"key : "<<i.key()<<" value : "<<i.value()<<endl;
                out<<"key : "<<QString::fromStdString(i.key())<<" value : "<<i.value()<<"\n";
            }
            file.close();
            break;
        }
        case 5:{
            string key;
            int value;
            cout<<"enter the key of element value of each you want to change"<<endl;
            cin>>key;
            cout<<"enter new value of element"<<endl;
            cin>>value;
            if(!Storage.contains(key)){
                cout<<"the key you entered don't exist"<< endl;
                break;
            }
            auto i=Storage.equal_range(key);
            for(auto it=i.first; it!=i.second; it++)
            it.value()=value;
            break;
        }
        case 6:{
            if (Storage.empty()){
                cout<<"Storage is empty"<<endl;
                break;
            }
            cout<<"Storage(map) right now: "<<endl;
            for (auto i=Storage.begin(); i!=Storage.end(); i++)
            {cout<<"key : "<<i.key()<<" value : "<<i.value()<<endl;}
            break;
        }
        case 7:{
            QFile file("C:/Users/HP/Documents/laba7/data.txt");
            if(!file.open(QIODevice::ReadWrite)){
                cout<<"eror in opening"<<endl;
                break;}
            QTextStream in(&file);
            while(!in.atEnd())
            {
                QString name;
                int age;
                in>>name;
                in>>age;
                Storage.insert(name.toStdString(), age);
            }
            file.close();
            break;
        }
        case 8:{
            cout<<"THE END OF PROGRAMME "<<endl;
            break;
        }
        }
    }while(k!=8);
    return 0;
}
