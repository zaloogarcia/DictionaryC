#Proyecto 1 
##**Recuperatorio**
### Traductor de textos
##### Daniel Bauer, Gonzalo Garcia.
---
##Introduccion

La idea de la traducción automática puede remontarse en la década de 1950, el experimento de Georgetown (1954) consistía en una traducción totalmente automática de más de sesenta oraciones del ruso al inglés. El experimento fue todo un éxito y marcó el comienzo de una era con una importante financiación para la investigación de tecnologías que permitiesen la traducción automática. 

A partir de los finales de la década de 1980, el poder de la computación aumentó la potencia de cálculo y la hizo menos costosa, y fue demostrado mayor interés en modelos estadísticos para la traducción automática.

Hoy en dia tenemos miles de traductores automaticos mas complejos para diferentes idiomas, que traducen palabras hasta frases complejas.

En este laboratorio, se implemento un traductor simple que cuenta con un diccionario al cual se consulta la traduccion de las palabras a traducir. En caso de no encontrar la palabra consultada, el usuario puede ignorarla o el mismo traducirla.
Es un traductor "simple" porque no puede traducir ni frases, ni omitir simbolos, solo consulta la traduccion a su diccionario en memoria.

---
##Compilacion

Para compilar simplemente:
`make`
Y para correr:  
`./bin/translator -i archivo.txt`

--- 
![keep-calm-i-am-a-translator.png](https://bitbucket.org/repo/LLGjg7/images/3046962964-keep-calm-i-am-a-translator.png)

##Implementacion

En las siguientes secciones se describe como fueron implementados las estructuras abstractas de datos y funciones de los modulos:

###Diccionario

Ubicado en el archivo `dict.h dict.c`, se lo implemento como un arbol binario, donde cada nodo tiene un puntero a una 'palabra' y otro su 'traduccion'.

`struct dict_t_ {`   
`  char *word;`    
`  char *trans;`   
`  dict_t left;`   
`  dict_t right;`   
};`  

Luego se implementaron funciones para la manipulacion de la estructura.
Con estas funciones se podran definir el diccionario personal y/o el diccionario a cargar.

Es necesario brindar este **dato curioso**, a la hora de almacenar las palabras y sus traducciones obtenidas de un archivo externo, a cada palabra o traduccion se lo almacena en un buffer de 252 bytes. Teniendo en cuenta que ninguna palabra debera superar este limite, siendo cada `char` 1 byte, la palabra no podra tener mas de 252 caracteres. Actualmente, la palabra (en Ingles) mas larga del mundo tiene 189819 letras. (Asi que por favor, no intenten traducir esa palabra) 

### Archivos

Ubicados en `file.c file.h`, proveen una estructura *"file"* que usamos para cargar el archivo de entrada y para crear el archivo de salida.
  
`struct file_array {`  
`  char **words;`   
`  int length;`   
`  int first;`   
};`  

Estos archivos tienen todas las funciones que lidian con *"files"*

### Ignorados

Ubicados en `ignored.c ignored.h`, en ellos se implementa la estructura `ign_t` que tendra las palabras a ignorar. Se lo implemento como arbol binario, similarmente como diccionario con la diferencia que el nodo contiene solamente una palabra.
   
`struct ign_t_ {`   
`  char *word;`   
`  ign_t left;`   
`  ign_t right;`   
`};`   

En lo que respecta de las funciones, son casi igual a las de diccionario.

### Main

En el main parseamos los argumentos que usa el usuario. El main usa todos estos archivos para cargar un archivo de entrada a memoria, cargar los diccionarios (el de traduccion y el de palabras ignoradas) y buscar las palabras en los diccionarios y traducirlas si es necesario. Luego imprime el texto traducido a un nuevo archivo.

---
### Nota de descargo
Queremos agradecer a la catedra por darnos una oportunidad de recuperar este (no tan querido) laboratorio. Tambien agradecemos por darnos tranquilidad, para terminar el proyecto, al haber postergado la entrega. 
---

##Bibliografia

 * [stackoverflow](http://stackoverflow.com/)
 * [tutorialspoint](http://www.tutorialspoint.com/)

   