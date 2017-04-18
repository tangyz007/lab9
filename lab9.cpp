/*
Name: Yunzhi Tang
ID:14255199
*/

#define MAX 100
#define number 26
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;


#define NUM_LETTERS 26
char Letters[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
string morsecode[] ={ ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };


class Message {
private:
// neither derived classes nor other users
// can access private members
        protected: char* input;

        public:
        virtual void print(void);
    	Message();// default constructor.
    Message(char* msg);// parametric constructor.
    ~Message();//destructor
    };

// Base class constructor
Message::Message() {
cout << "Please Enter a message: "<<endl;
input= new char[MAX];
cin>>input;
cout<<input<<endl;
}



Message::Message(char* msg) {
input= new char[MAX];
strcpy(input,msg);
if(input == NULL)
cerr << "Error in memory allocation";
}

Message::~Message() {
delete[] input;
}


void Message::print(void) {
cout << "Orignal text : " << input << endl;

}



class morseCodeMessage : public Message { // morseCodeMessage is derived from class Message
        protected:
	string *morse_msg;
	    void translate();
    	private:


        public:
                // Override virtual:
                void print();
                morseCodeMessage();
                ~morseCodeMessage();
                void LED (void);
                //void initial(void);
};

// Base class constructor
morseCodeMessage::morseCodeMessage() {
//cout << "Initializing morse code generator "<<endl;
morse_msg= new string[strlen(input)];
//cin>>input;
//cout<<input<<endl;
translate();
print();
}

morseCodeMessage::~morseCodeMessage() {
	delete[] morse_msg;
}

void morseCodeMessage::LED (void)
{
	cout << "Original text: " << input << endl;
	        cout << "Morse code: ";
	        for(int i=0; i<strlen(input); i++)
	        {
	        cout << morse_msg[i];
	        }
	        cout << endl;

	int fd;// for the file descriptor of the special file we need to open.
	                unsigned long *baseptr;// base pointer, for the beginning of the memory page (mmap)
	                unsigned long *PBDR,*PBDDR;// pointers for port B DR/DDR
	                fd = open("/dev/mem", O_RDWR|O_SYNC);   // open the special file /dem/mem
	                        if(fd == -1){
	                                printf("\nError: cannot open /dev/mem.\nAre you root?\n");
	                                exit(-1);  // failed open
	                        }

	                        // We need to map Address 0x80840000 (beginning of the page)
	                        baseptr = (unsigned long*)mmap(NULL,getpagesize(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
	                        if(baseptr == MAP_FAILED){
	                                printf("\nError: Unable to map memory space \n");
	                                exit(-2);
	                        }  // failed mmap
	                        close(fd);


	                        PBDR = baseptr + 1;     // Address of port B DR is 0x80840004
	                        PBDDR = baseptr + 5;    // Address of port B DDR is 0x80840014
	                        for(int j=0; j<strlen(input); j++)
	                                {
	                                        string copy = morse_msg[j];

	                                        for(int k=0; k<copy.length(); k++)
	                                        {
	                                                //sleep(1);
	                                                char c = copy[k];
	                                                if(c == '.')
	                                                {
	                                                        // RED LED => B5

	                                                        *PBDR |= 0x20;
	                                                        cout << "Red" << endl;

	                                                        sleep(1);
	                        //usleep(500000);//sleep for .5sec
	                                                        *PBDR &= ~0x20;
	                                                }
	                                                else if(c == '-')
	                                                {
	                                                        // YELLOW LED => B6

	                                                        *PBDR |= 0x40;
	                                                        cout << "Yellow" << endl;

	                                                        sleep(1);
	                                                        //usleep(500000);//sleep for .5sec

	                                                        *PBDR &= ~0x40;
	                                                }
	                                                else
	                                                {
	                                                        cout << "Character not morse code: " << c << endl;
	                                                }
	                                        }
	                                        sleep(1);
	                                        //usleep(500000);//sleep for .5sec
	                                }
	                                *PBDR |= 0x80;
	                                cout << "Green" << endl;
	                                sleep(1);
	                                *PBDR |= 0x80;

	                                *PBDR &= 0x00;
	                        }





void morseCodeMessage::translate() {
for(int i=0; i<strlen(input); i++)
{
for(int j=0; j<number; j++)
{
if(*(input+i) == *(Letters+j))
{
morse_msg[i] = morsecode[j];
break;
}
}
}
}

void morseCodeMessage::print() {
cout << "Original text: " << input << endl;
cout << "Morse code: ";
for(int i=0; i<strlen(input); i++)
{
cout << morse_msg[i];
}
cout << endl;
cout << endl;
}








int main(int argc, char **argv) {
        int fd;         // for the file descriptor of the special file we need to open.
                unsigned long *BasePtr;         // base pointer, for the beginning of the memory page (mmap)
                unsigned long *PBDR, *PBDDR;    // pointers for port B DR/DDR
                fd = open("/dev/mem", O_RDWR|O_SYNC);   // open the special file /dem/mem
                        if(fd == -1){
                                printf("\n error\n");
                            return(-1);  // failed open
                        }
                        // We need to map Address 0x80840000 (beginning of a memory page)
                                // An entire memory page is mapped. We need to point to the beginning of the page.
                                BasePtr = (unsigned long*)mmap(NULL,getpagesize(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
                            if(BasePtr == MAP_FAILED){
                                  printf("\n Unable to map memory space \n");
                                  return(-2);
                            }  // failed mmap

                        PBDR = BasePtr + 1;
                         PBDDR = BasePtr + 5;
                        *PBDDR &= 0xFFFFFFFE;
                        *PBDDR &= 0xFFFFFFF8;


        //Message l1 = morseCodeMessage();
        morseCodeMessage l2;
        l2.LED();
int flag=1;
//while(flag);
{
cout<<"press any button!"<<endl;
int a;
int b;
int c;
a=*PBDR&0x01;
b=*PBDR&0x02;
c=*PBDR&0x03;
if(a==0){
        cout<<"the button 1 is pressed"<<endl;
cout<<"-"<<endl;
}
if(a==1){
        cout<<"the button 1 is not pressed"<<endl;
}
if(b==0){
        cout<<"the button 2 is pressed"<<endl;
cout<<"."<<endl;
}
if(b==1)
        cout<<"the button 2 is not pressed"<<endl;
if(c==0){
        cout<<"the button 3 is pressed"<<endl;
flag=0;
}
//int var = *PBDR & 0x01;
//cout << "Button Val:: "<< var <<endl;
}
        return 0;
}

