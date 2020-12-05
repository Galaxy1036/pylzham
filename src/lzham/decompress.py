from . import _lzham


class LZHAMDecompressor:

    def __init__(self, filters=None):
        self.kwargs = {}
        self._reinit = False
        self.parse_filters(filters)
        self._handler = _lzham.lzham_decompress_init(**self.kwargs)

    def reinit(self, filters=None):
        self.parse_filters(filters)
        self._handler = _lzham.lzham_decompress_reinit(self._handler, **self.kwargs)
        self._reinit = False

    def parse_filters(self, filters):
        if filters is not None:
            _args = [
                     'dict_size_log2',
                     'table_update_rate',
                     'table_max_update_interval',
                     'table_update_interval_slow_rate',
                     'compute_adler32_during_decomp',
                     'unbuffered_decompression'
                     ]

            for arg in _args:
                if filters.get(arg) is not None:
                    self.kwargs[arg] = filters[arg]

    def decompress(self, data, uncompressed_size):
        if self._reinit:
            self.reinit(self.kwargs)

        self._reinit = True
        return _lzham.lzham_decompress(self._handler, data, uncompressed_size)


def decompress(data, uncompressed_size, filters=None):
    return LZHAMDecompressor(filters).decompress(data, uncompressed_size)
