# JSON Parser

## **Autore**
> **Alberto Campagnolo: 897569**

## **Descrizione**
Questo progetto implementa un **parser per file JSON**, in grado di gestire sei tipi di dati principali in input:
- `null`
- `double`
- `bool`
- `string`
- `list`
- `dictionary`

Il parser utilizza `json.hpp`, una libreria fornita da [nicolaprezza/progetto_PEL_22_23](https://github.com/nicolaprezza/progetto_PEL_22_23) come header di riferimento. Il file **Laureate** è un'ottima base per effettuare test sulla gestione dei dati JSON.

## **Obiettivi**
Il progetto affronta tre sfide principali:
1. **Lettura e parsing** dei dati JSON da un file di input.
2. **Conversione** dei dati nel tipo corrispondente.
3. **Gestione delle eccezioni** per garantire robustezza e correttezza nell'elaborazione.

## **Struttura del codice**
- **`json.hpp`** → Contiene la dichiarazione della classe `json`, con metodi per manipolare i diversi tipi di dati JSON.
- **`json.cpp`** → Implementa le funzionalità definite in `json.hpp`, inclusi metodi di parsing, accesso ai dati e gestione degli iteratori.
- **`in.json`** → File JSON di input da analizzare.
- **`out.json`** → File JSON di output generato dal parser.

## **Esempi di utilizzo**
### **Accesso agli elementi JSON**
```cpp
json j;
{
  "prima stringa" : [1,2,3],
  "seconda stringa" : 6 
}

j["prima stringa"]; // Ritorna un oggetto contenente [1,2,3] (list)
j["seconda stringa"]; // Ritorna un oggetto contenente 6 (number)
j["terza stringa"]; // Ritorna la coppia chiave-valore ("terza stringa", null)
```

### **Iterazione su una lista JSON**
```cpp
json j2;
// Inserisco in j2 [1,2,[1,2,3]]

auto it = j2.begin_list();
*it; // Ritorna il primo valore JSON
*(++it); // Ritorna il secondo valore JSON
```

## **Riferimenti**
- [Repository originale di json.hpp](https://github.com/nicolaprezza/progetto_PEL_22_23)
---