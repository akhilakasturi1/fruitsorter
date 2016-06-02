 /*
Here the variables  u,a,b,c,d are used to remember the number of fruits of the Grade : Undefined,A,B,C,D detected.
                    a2,b2,c2,d2 (isSet) are for setting the sorter back to its stable position after current fruit is sorted.
                    mechanismDelay(milliseconds) is the amount of time the mechanism takes between giving command to set itself and actually setting properly
                    t is the amount of delay time between imager and sorter and defDelayTime(milliseconds) is its default value
                    del(milliseconds) holds the value after which we are sure the fruit has fallen into the right grade basket
                    sort is used to handle incoming fruit grades

                    t must always be greater than mechanismDelay; look at their definitions above.
*/

int defDelayTime=6000, mechanismDelay = 200;
unsigned long t=0;
unsigned long setTime[50];
int nextToBeSet=0;
int nextToBeDeleted=0;
signed long del=2000;
char sort;
int canBeServiced=1;
char fruitList[50];
int lastFruit=0;
int fruitQuant[5]={0,0,0,0,0};//Represents 'A' 'B' 'C' 'D' 'E'

void setGrade(char s)
{
    switch(s)
    {
        case 'A':   //North
            Serial.println("Grade A");
            digitalWrite(22,HIGH);
            digitalWrite(23,LOW);
            digitalWrite(24,LOW);
            digitalWrite(25,HIGH);
            digitalWrite(26,LOW);
            digitalWrite(27,HIGH);
            digitalWrite(28,LOW);
            digitalWrite(29,HIGH);
            break;
        case 'B':   //South
            Serial.println("Grade B");
            digitalWrite(22,LOW);
            digitalWrite(23,HIGH);
            digitalWrite(24,HIGH);
            digitalWrite(25,LOW);
            digitalWrite(26,LOW);
            digitalWrite(27,HIGH);
            digitalWrite(28,LOW);
            digitalWrite(29,HIGH);
            break;
        case 'C':   //East
            Serial.println("Grade C");
            digitalWrite(22,LOW);
            digitalWrite(23,HIGH);
            digitalWrite(24,LOW);
            digitalWrite(25,HIGH);
            digitalWrite(26,LOW);
            digitalWrite(27,HIGH);
            digitalWrite(28,HIGH);
            digitalWrite(29,LOW);
            break;
        case 'D':   //West
            Serial.println("Grade D");
            digitalWrite(22,LOW);
            digitalWrite(23,HIGH);
            digitalWrite(24,LOW);
            digitalWrite(25,HIGH);
            digitalWrite(26,HIGH);
            digitalWrite(27,LOW);
            digitalWrite(28,LOW);
            digitalWrite(29,HIGH);
            break;
        default:   //stable
            Serial.println("Stable");
            digitalWrite(22,LOW);
            digitalWrite(23,HIGH);
            digitalWrite(24,LOW);
            digitalWrite(25,HIGH);
            digitalWrite(26,LOW);
            digitalWrite(27,HIGH);
            digitalWrite(28,LOW);
            digitalWrite(29,HIGH);
    }
}

void setup() {
    Serial.begin(9600);
    delay(2000);
    Serial.println("Enter delay from imager to sorter if available");
    Serial.print("Waiting...");
    while (millis() < 3000)
    {
        if(Serial.available() > 0)
        {
            t*=10;
            t+=(Serial.read() - 48); //Because Serial.read can read only one byte at a time
        }
    }
    if(t==0)
    {
        t=defDelayTime;
    }
    else
    {
      t*=10; //To convert to milliseconds
    }
    Serial.print("Chosen delay time is : ");
    Serial.print(t);
    Serial.println(" milliseconds.");
    pinMode(22,OUTPUT);
    pinMode(23,OUTPUT);
    pinMode(24,OUTPUT);
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
    pinMode(27,OUTPUT);
    pinMode(28,OUTPUT);
    pinMode(29,OUTPUT);
    setGrade('E');  //To set it to stable position. Grade 'E' activates default in setGrade()
}

void loop() {
    if(Serial.available()>0)
    {
      fruitList[lastFruit++]=Serial.read();
      if(lastFruit==50)lastFruit=0;
    }

    if(analogRead(A4)<450 && canBeServiced==0)
    {
      delay(2);
      Serial.println(analogRead(A4));
      if(analogRead(A4)<450)
      {
        canBeServiced=1; 
      }
    }
    
    if((canBeServiced==1) && (analogRead(A4)>450) && (lastFruit-(fruitQuant[0]+fruitQuant[1]+fruitQuant[2]+fruitQuant[3]+fruitQuant[4]))>0)
    {
      canBeServiced=0;
      sort = fruitList[(fruitQuant[0]+fruitQuant[1]+fruitQuant[2]+fruitQuant[3]+fruitQuant[4])%50];
      setTime[(fruitQuant[0]+fruitQuant[1]+fruitQuant[2]+fruitQuant[3]+fruitQuant[4])%50] = (millis()+t);
      if(sort=='A' || sort=='B' || sort=='C' || sort=='D')
        fruitQuant[sort-65]++;
      else
        fruitQuant[4]++;
      Serial.print(sort);
      Serial.println(" serviced...");
    }

    if(((setTime[nextToBeSet] - millis()) < mechanismDelay) && nextToBeSet<((fruitQuant[0]+fruitQuant[1]+fruitQuant[2]+fruitQuant[3]+fruitQuant[4])%50))
    {
      setGrade(fruitList[nextToBeSet]);
      nextToBeSet++;
    }
    if((nextToBeDeleted<nextToBeSet) && ((signed long)(millis() - setTime[nextToBeDeleted]) > del))
    {
      if(fruitList[nextToBeDeleted+1]!=fruitList[nextToBeDeleted])
        setGrade('E');
      nextToBeDeleted++;
    }
    
    if((millis()%60000) < 5)  //After every 1 minute, accounting for a 5 millisecond program runtime to get to this if statement
    {
        Serial.println();
        Serial.println();
        Serial.print("After ");
        Serial.print(millis()/3600000);
        Serial.print("hr(s) and ");
        Serial.print((millis()%3600000)/60000);
        Serial.println("mins : ");
        Serial.println();
        Serial.println(" Objects Scanned ");
        Serial.print("GRADE A: ");
        Serial.println(fruitQuant[0]);
        Serial.print("GRADE B: ");
        Serial.println(fruitQuant[1]);
        Serial.print("GRADE C: ");
        Serial.println(fruitQuant[2]);
        Serial.print("GRADE D: ");
        Serial.println(fruitQuant[3]);
        Serial.print("Unknown : ");
        Serial.println(fruitQuant[4]);
    }
}

