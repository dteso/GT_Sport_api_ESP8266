/************************************* Clock *************************************/

int nuevoValor;
int valorActual;
int realCurrentValue;

//Allows control to carry out actions once in the same second
boolean instantChange;

int sc, mn, hr;

void clk()
{
 nuevoValor = (millis() / 1000);
 realCurrentValue = millis();
 if (nuevoValor != valorActual)
  {
    valorActual = nuevoValor;
    sc++;
    instantChange=true;
    if (sc == 60)
    {
      sc=0;
      mn++;
    //Every Minute Functions....
    }

    if (mn == 60)
    {
        mn = 0;
        hr++;
    }
    //Every Second Functions....
    Serial.print(hr);Serial.print(":");Serial.print(mn);Serial.print(":");Serial.println(sc);     
   }
  else{
    instantChange = false;
  }
}
