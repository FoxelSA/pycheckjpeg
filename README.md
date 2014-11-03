
## pycheckjpeg<br />A python module to check integrity of JPEG files

pycheckjpeg can validate a JPEG file from a buffer or a path, it returns 0 if the file is valid, else the error count (>0)

### Installation

Open a terminal and type the following commands

    python setup.py build
    sudo python setup.py install
    
### Usage examples

```python
from pycheckjpeg import validate_jpeg

errors = validate_jpeg('file.jpeg')

if errors:
    print("Image corrupted: ")
    
    for err in errors:
        print(err)
else:
    print("Tests passed")

```


```python
from pycheckjpeg import validate_jpeg_from_buffer

with open('file.jpeg', 'rb') as image:
    image_data = image.read()
    errors = validate_jpeg_from_buffer(image_data, len(image_data))
    
    if errors:
        print("Image corrupted: ")
        
        for err in errors:
            print(err)
    else:
        print("Tests passed")

```

### Copyright

Copyright (c) 2013-2014 FOXEL SA - [http://foxel.ch](http://foxel.ch)<br />
This program is part of the FOXEL project <[http://foxel.ch](http://foxel.ch)>.

Please read the [COPYRIGHT.md](COPYRIGHT.md) file for more information.


### License

This program is licensed under the terms of the
[GNU Affero General Public License v3](http://www.gnu.org/licenses/agpl.html)
(GNU AGPL), with two additional terms. The content is licensed under the terms
of the
[Creative Commons Attribution-ShareAlike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)
(CC BY-SA) license.

You must read <[http://foxel.ch/license](http://foxel.ch/license)> for more
information about our Licensing terms and our Usage and Attribution guidelines.
