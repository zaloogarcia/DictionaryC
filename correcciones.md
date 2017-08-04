# Correcciones

## Puntos Incorrectos

- No se sabe que palabra está traduciendo.
- Lanza un core dump cuando se ejecutan varias veces la opción h o segmentation
  fault antes de finalizar.
- Toma la entrada de la traducción como la opción siguiente y lanza errores.
- No permite la opción t (la pusieron como opción l)
- Es difícil saber si funciona porque nunca puedo ver el archivo de salida
  final, siempre lanza algún tipo de coredump.
- Pierde mucha memoria, probablemente por el segmentation fault.
- El README está mal ubicado (debía estar dentro de la carpeta Lab 1)
- La idea era que hicieran el tad de diccionario ustedes. La GLib no es mala
  opción pero hace las cosas más complejas y un tad de una HashTable no es tan
  complicado de hacer.
- No puedo probar si funciona reverse por los core dumps. En general no puedo
  probar si funciona alguna otra opción.

## Observaciones

No terminé de ver todos los errores con detalle más que nada porque no puedo,
los abort que lanza por culpa de la GLib que no funciona. Es una lástima porque
se veía prometedor el código.

## Nota final: 3 (Reprobado)
