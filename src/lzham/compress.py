from . import _lzham


class LZHAMCompressor:

    def __init__(self, filters=None):
        self.kwargs = {}
        self._reinit = False
        self.parse_filters(filters)
        self._handler = _lzham.lzham_compress_init(**self.kwargs)

    def reinit(self):
        self._handler = _lzham.lzham_compress_reinit(self._handler)
        self._reinit = False

    def parse_filters(self, filters):
        if filters is None:
            filters = {}

        else:
            _args = [
                     'dict_size_log2',
                     'level',
                     'table_update_rate',
                     'max_helper_threads',
                     'table_max_update_interval',
                     'table_update_interval_slow_rate'
                     ]

            for arg in _args:
                if filters.get(arg) is not None:
                    self.kwargs[arg] = filters[arg]

    def compress(self, data):
        if self._reinit:
            self.reinit()

        self._reinit = True
        return _lzham.lzham_compress(self._handler, data)


def compress(data, filters=None):
    return LZHAMCompressor(filters).compress(data)
