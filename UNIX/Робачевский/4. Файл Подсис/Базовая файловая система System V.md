
## Введение

Исконной файловой системой(ФС) UNIX System V является **s5fs**.
**FFS**(BSD) появилась позже, но она обладает лучшей производительностью, функциональностью и надежностью.

Когда появилась **FFS**, UNIX поддерживала работу с одним типом файловой системы(ФС). Таким образом, создатели различных версий ОС UNIX вынуждены были выбирать только одну ФС. Это решилось введением ***независимой(виртуальной) файловой системы***, позволявшей работать с несколькими "физическими" файловыми системами разных типов.

## Базовая файловая система System V

>[!note] Определение
>Каждый жесткий диск состоит из одной или нескольких логических частей, называемых разделами (partitions).

Расположение и размер раздела определяются при форматировании диска
В UNIX разделы выступают в качестве независимых устройств, доступ к которым осуществляется как к различным носителям данных.

>[!example] Пример
>Диск может состоять из четырех разделов, каждый из которых содержит свою файловую систему. Заметим, что в разделе может располагаться только одна файловая система, которая не может занимать несколько разделов. В другой конфигурации диск может состоять только из одного раздела, позволяя создание весьма емких файловых систем.

![[Pasted image 20240917214638.png]]

- ***Суперблок*** (Superblock) - содержит общую информацию о ФС, например, об ее архитектуре, общем числе блоков и индексных дескрипторов, или метаданных (inode)
- ***Массив индексных дескрипторов*** (ilist). Содержит метаданные всех файлов ФС. Индексный дескриптор содержит статусную информацию о файле и указывает на расположение данных этого файла. Ядро обращается к inode по индексу в массиве ilist. Один inode является корневым (root) inode ФС, через него обеспечивается доступ к структуре каталогов и файлов после монтирования ФС. Размер массива ilist является фиксированным и задается при создании ФС. Таким образом, ФС **s5fs** имеет ограничение по числу файлов, которые могут храниться в ней, независимо от размера этих файлов.
- ***Блоки хранения данных***. Данные обычных файлов и каталогов хранятся в блоках. Обработка файла осуществляется через inode, содержащего ссылки на блоки данных. Блоки ХД занимают большую часть дискового раздела, и их число определяет максимальный суммарный объем файлов данной ФС. Размер блока кратен `512` байтам.