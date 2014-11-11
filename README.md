
## pycheckjpeg<br />A python module to check integrity of JPEG files

pycheckjpeg can validate a JPEG file from a buffer or a path, it returns error and warning messages if anything is found.

### Dependencies

 _libjpeg-dev_

    sudo apt-get install libjpeg-dev


### Installation

Open a terminal and type the following commands

    python setup.py build
    sudo python setup.py install

### Data structure

When you call pycheckjpeg functions it returns a two dimentional array with warning and error messages

    [
        [], # Error messages
        []  # Warning messages
    ]

### Usage examples

```python
from pycheckjpeg import validate_jpeg_from_file

# Verify image
messages = validate_jpeg_from_file('file.jpeg')

# Check if image have errors
if messages[0]:

    # Display error messages
    print("Image have errors")

    for err in messages[0]:
        print(err)

# Check if image have warnings
if messages[1]:

    # Display warning messages
    print("Image have warnings")

    for warn in messages[1]:
        print(warn)

```


```python
from pycheckjpeg import validate_jpeg_from_buffer

# Open file
with open('file.jpeg', 'rb') as image:

    # Read image data
    image_data = image.read()

    # Verify image
    messages = validate_jpeg_from_buffer(image_data)

    # Check if image have errors
    if messages[0]:

        # Display error messages
        print("Image have errors")

        for err in messages[0]:
            print(err)

    # Check if image have warnings
    if messages[1]:

        # Display warning messages
        print("Image have warnings")

        for warn in messages[1]:
            print(warn)
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
