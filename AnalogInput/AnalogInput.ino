// inclusão de bibilioteca de estatística
#include "QuickStats.h"
#include "ArduinoSort.h"

unsigned int i=0; // contador de conversões AD
const int n = 400; // número total de medições
float temperatura[n], temperaturaRef[n]; //dados com e sem ruído, respectivamente 
float desvios[n], m, mediana, s, mFinal, sFinal, tempref, dpadref;
QuickStats stats; // objeto do tipo QuickStats
void setup() {
  // inicialização da comunicação serial
  Serial.begin(9600);
}

//Z Score 

void zscore(float dados[n])
{
  m = stats.average (dados, n);
  s = stats.stdev(dados, n);
  int j = 0;

  for (int i = 0; i<n; i++)
  {
    desvios[i] = abs(dados[i] - m);    
    if (desvios[i] <= 3*s)
    {
      j++;
    }     
  }
  float dadosFiltrados[j];
  int t = 0;
  for (int i = 0; i < n; i++)
  {
    if (desvios[i] <= 3*s)
    {
      dadosFiltrados[t] = dados[i];
      t++;
    }     
  }
  
  // resultado final
  mFinal = stats.average(dadosFiltrados, j);
  sFinal = stats.stdev(dadosFiltrados, j);

  
  Serial.println("T1(°C) T2(°C) desvio(°C)");
  for(int x=0; x < n; x++)
  {  
    Serial.print(temperaturaRef[x]);
    Serial.print(" ");
    Serial.print(temperatura[x]);
    Serial.print(" ");
    Serial.println(desvios[x]);       
  }    

  Serial.println("--------------------------TEMPERATURA DE REFERÊNCIA---------------------------------"); 
  Serial.print("indicação média: ");
  Serial.print(tempref);
  Serial.println(" °C");
  Serial.print("desvio padrão: ");
  Serial.print(dpadref);
  Serial.println(" °C");
  Serial.println("----------------------------SINAL + RUÍDO-------------------------------------------");
  Serial.print("indicação média:");  
  Serial.print(m);
  Serial.println(" °C");
  Serial.print("desvio padrão:");  
  Serial.print(s);
  Serial.println(" °C");
  Serial.println("------------------------------------------------------------------------------------");
  
  Serial.println("----------------------------------Z SCORE-------------------------------------------");
  Serial.print("número de dados retirados:");
  Serial.println(n-j);
  Serial.print("intervalo: de ");
  Serial.print(m - 3*s);
  Serial.print(" °C até ");
  Serial.print(m + 3*s);
  Serial.println(" °C");
  
  /*Serial.println("dados filtrados:");
  Serial.println("Temp (°C)");
  for(int x=0; x < j; x++)
  {
    Serial.println(dadosFiltrados[x]);  
  } */
  Serial.print("indicação média:");  
  Serial.print(mFinal);
  Serial.println(" °C");
  Serial.print("desvio padrão:");  
  Serial.print(sFinal);
  Serial.println(" °C");
  Serial.println("------------------------------------------------------------------------------------");
  

}

//Chauvenet

float chauvenet(float dados[n])
{
  
  m = stats.average (dados, n);
  s = stats.stdev(dados, n);
  int j = 0;

  for (int i = 0; i < n; i++)
  {
    desvios[i] = abs(dados[i] - m);    
    if (desvios[i] < 3.22*s)
    {
      j++;
    }     
  }
  float dadosFiltrados[j];
  int t = 0;
  for (int i = 0; i < n; i++)
  {
    if (desvios[i] < 3.22*s)
    {
      dadosFiltrados[t] = dados[i];
      t++;
    }     
  }
  
  // resultado final
  mFinal = stats.average(dadosFiltrados, j);
  sFinal = stats.stdev(dadosFiltrados, j);

  Serial.println("---------------------------------CHAUVENET------------------------------------------");
  Serial.print("número de dados retirados:");
  Serial.println(n-j);
  Serial.print("intervalo: de ");
  Serial.print(m - 3.22*s);
  Serial.print(" °C até ");
  Serial.print(m + 3.22*s);
  Serial.println(" °C");
  /*Serial.println("dados filtrados:");
  Serial.println("Temp (°C)");
  for(int x=0; x < j; x++)
  {
    Serial.println(dadosFiltrados[x]);  
  }*/
  Serial.print("indicação média:");  
  Serial.print(mFinal);
  Serial.println(" °C");
  Serial.print("desvio padrão:");  
  Serial.print(sFinal);
  Serial.println(" °C");
  Serial.println("------------------------------------------------------------------------------------");
}

//MAD
float madev(float dados[n])
{
  mediana = stats.median (dados, n);
  s = stats.stdev(dados, n);
  int j = 0;
  
  for (int i = 0; i<n; i++)
  {
    desvios[i] = abs(dados[i] - mediana);        
  }    
  float MAD = 1.4826 * stats.median(desvios, n);

  for (int i = 0; i < n; i++)
  {
    if (desvios[i] <= 3*MAD  )
    {
      j++;
    }     
  }
  
  float dadosFiltrados[j];
  int t = 0;
  for (int i = 0; i < n; i++)
  {
    if (desvios[i] <= 3*MAD )
    {
      dadosFiltrados[t] = dados[i];
      t++;
    }     
  }
  
  // resultado final
  mFinal = stats.average(dadosFiltrados, j);
  sFinal = stats.stdev(dadosFiltrados, j);

  Serial.println("---------------------------------------MAD------------------------------------------");
  Serial.print("número de dados retirados:");
  Serial.println(n-j);
  Serial.print("intervalo: de ");
  Serial.print(mediana - 3*MAD);
  Serial.print(" °C até ");
  Serial.print(mediana + 3*MAD);
  Serial.println(" °C");
  /*Serial.println("dados filtrados:");
  Serial.println("Temp (°C)");
  for(int x=0; x < j; x++)
  {
    Serial.println(dadosFiltrados[x]);  
  }*/
  Serial.print("indicação média:");  
  Serial.print(mFinal);
  Serial.println(" °C");
  Serial.print("desvio padrão:");  
  Serial.print(sFinal);
  Serial.println(" °C");
  Serial.println("------------------------------------------------------------------------------------");

}

//Interquartil
float interquartil(float dados[n])
{
  sortArray(dados, n);
  float IQR, q75, q25, linf, lsup;
  q75 = (dados[299]+dados[300])/2;
  q25 = (dados[99]+dados[100])/2;
  IQR = q75 - q25;
  linf = q25 - 1.5 * IQR;
  lsup = q75 + 1.5* IQR;
  
  int j = 0;
 
  for (int i = 0; i < n; i++)
  {
    if (dados[i] >= linf && dados[i] <= lsup )
    {
      j++;
    }     
  }
  
  float dadosFiltrados[j];
  int t = 0;
  for (int i = 0; i < n; i++)
  {
    if (dados[i] >= linf && dados[i] <= lsup)
    {
      dadosFiltrados[t] = dados[i];
      t++;
    }     
  }
  
  // resultado final
  mFinal = stats.average(dadosFiltrados, j);
  sFinal = stats.stdev(dadosFiltrados, j);

  Serial.println("---------------------------------INTERQUARTIL---------------------------------------");
  Serial.print("número de dados retirados:");
  Serial.println(n-j);
  Serial.print("intervalo: de ");
  Serial.print(linf);
  Serial.print(" °C até ");
  Serial.print(lsup);
  Serial.println(" °C");
  /*Serial.println("dados filtrados:");
  Serial.println("Temp (°C)");
  for(int x=0; x < j; x++)
  {
    Serial.println(dadosFiltrados[x]);  
  }*/
  Serial.print("indicação média:");  
  Serial.print(mFinal);
  Serial.println(" °C");
  Serial.print("desvio padrão:");  
  Serial.print(sFinal);
  Serial.println(" °C");
  Serial.println("------------------------------------------------------------------------------------");

}

void loop() {

  if (i<1)
  {
    i++;
  
    for(int j=0; j < n; j++)
    { 
      temperatura[j]= analogRead(A0)/1023.0*500.0;
      temperaturaRef[j]= analogRead(A1)/1023.0*500.0;
      delay(1);
    }

    tempref = stats.average(temperaturaRef, n);
    dpadref = stats.stdev(temperaturaRef, n);
    
    zscore(temperatura);
    chauvenet(temperatura);
    madev(temperatura);
    interquartil (temperatura);
  }
}
