# PyLZHAM
**PyLZHAM** is a python 3 wrapper for the LZHAM Compression Codec

## Installation
In order to install pylzham, you'll simply need to use the following command
> python -m pip install pylzham

**Note**: PyLZHAM use C++ extension which means python need a compiler to build this module
## Usage
### Compression
You can easily compress data using the following snippet:

    >>> import lzham
    >>> lzham.compress(b'yourdatahere' * 100)
    b'@\x0b\x9f\x85\x07\x96\xf7W&F\x17F\x16\x86P\x07&W\x98(C\xf4\x03\xf7\xf4\x02m\x98\xc0P\xeb\xf9$'

However if you need to compress many files/data, using the `LZHAMCompressor` class might be a better idea. Here is a quick example:

    >>> from lzham import LZHAMCompressor
    >>> compressor = LZHAMCompressor()
    >>> compressor.compress(b'yourdatahere' * 100)
    b'@\x0b\x9f\x85\x07\x96\xf7W&F\x17F\x16\x86P\x07&W\x98(C\xf4\x03\xf7\xf4\x02m\x98\xc0P\xeb\xf9$'
    >>> compressor.compress(b'yourotherdata' * 100)
    b'@\x07\xcd\xd9\x07\x96\xf7W&\xf7F\x86W&D\xc6\x17F\x17\x98(D\x03\xf7\xf4\x03\xf7\xf4\x01\x98\xc03i*k'

##### Using compression option
If you wanna set compression options you should use a compression filter. Filters support the following options (specified as additional entries in the dictionary representing the filter):

- `dict_size_log2`
- `level`
- `table_update_rate`
- `max_helper_threads`
- `table_max_update_interval`
- `table_update_interval_slow_rate`

For more information about thoses options you can look [there](https://github.com/richgel999/lzham_codec/wiki/LZHAM-Codec-API-Reference#compression-parameters-struct).

Here is an example of using filters with both `lzham.compress` and `LZHAMCompressor`:

     >>> filters = {'dict_size_log2': 18}
     >>> lzham.compress(b'yourdata', filters)
     >>> compressor = lzham.LZHAMCompressor(filters)
     >>> compressor.compress(b'yourdata')

###Decompression
You can easily decompress data using the following snippet:

    >>> import lzham
    >>> lzham.decompress(b'D\xad\xc0\x00\x07FW7@\x07i1\x98\xc0f\xb1\x11\x81', 40)
    b'testtesttesttesttesttesttesttesttesttest'

**Note**: Since there is no offical header for LZHAM you have to give the decompressed data size to the lib (`40` in our case). 

However if you need to decompress many files/data, using the `LZHAMDeompressor` class might be a better idea. Here is a quick example:

    >>> from lzham import LZHAMDecompressor
    >>> decompressor = LZHAMDecompressor()
    >>> decompressor.decompress(b'D\xad\xc0\x00\x07FW7@\x07i1\x98\xc0f\xb1\x11\x81', 40)
	b'testtesttesttesttesttesttesttesttesttest'
	>>> decompressor.decompress(b'D\xad\xc0\x00\x07FW7@\x07i1\x98\xc0f\xb1\x11\x81', 40)
	b'testtesttesttesttesttesttesttesttesttest'

##### Using decompression option
If you wanna set decompression options you should use a decompression filter. Filters support the following options (specified as additional entries in the dictionary representing the filter):

- `dict_size_log2`
- `table_update_rate`
- `table_max_update_interval`
- `table_update_interval_slow_rate`
- `compute_adler32_during_decomp`
- `unbuffered_decompression`

For more information about thoses options you can look [there](https://github.com/richgel999/lzham_codec/wiki/LZHAM-Codec-API-Reference#decompression-parameters-struct).

Here is an example of using filters with both `lzham.decompress` and `LZHAMDecompressor`:

     >>> filters = {'dict_size_log2': 18}
     >>> lzham.decompress(b'D\xad\xc0\x00\x07FW7@\x0fi3\x98\xc0f\xb1\x11\x81', 40, filters)
	>>> decompressor = lzham.LZHAMDecompressor(filters)
	>>> decompressor.decompress(b'D\xad\xc0\x00\x07FW7@\x0fi3\x98\xc0f\xb1\x11\x81', 40)

If you need to modify your `LZHAMDecompressor` instance filters you can simply call its `reinit` function like this:

	>>> decompressor.reinit(filters)

## Note

I'm a C++ newbie so my code might be really trash. If you have any bug to report feel free to contact me at @GaLaXy1036#1601 on Discord.