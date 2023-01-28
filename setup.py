import platform

from setuptools import setup, Extension


ext_sources = [
               'src/lzham_module.cpp',
               'src/lib/lzham_api.cpp',
               'src/lib/lzham_assert.cpp',
               'src/lib/lzham_checksum.cpp',
               'src/lib/lzham_huffman_codes.cpp',
               'src/lib/lzham_lzbase.cpp',
               'src/lib/lzham_lzcomp.cpp',
               'src/lib/lzham_lzcomp_internal.cpp',
               'src/lib/lzham_lzcomp_state.cpp',
               'src/lib/lzham_lzdecomp.cpp',
               'src/lib/lzham_lzdecompbase.cpp',
               'src/lib/lzham_match_accel.cpp',
               'src/lib/lzham_mem.cpp',
               'src/lib/lzham_platform.cpp',
               'src/lib/lzham_prefix_coding.cpp',
               'src/lib/lzham_symbol_codec.cpp',
               'src/lib/lzham_timer.cpp',
               'src/lib/lzham_vector.cpp',
               ]

if platform.system() == 'Windows':
    ext_sources.append('src/lib/lzham_win32_threading.cpp')

else:
    ext_sources.append('src/lib/lzham_pthreads_threading.cpp')

with open('README.md', 'r') as f:
    long_description = f.read()

setup(name='pylzham',
      version='0.1.3',
      author='GaLaXy1036',
      description='Python 3 Wrapper for LZHAM Codec',
      long_description=long_description,
      long_description_content_type='text/markdown',
      url='https://github.com/Galaxy1036/pylzham',
      ext_modules=[Extension('_lzham',
                             ext_sources)],
      ext_package='lzham',
      package_dir={'': 'src'},
      packages=['', 'lzham'],
      license='MIT',
      classifiers=[
                   'Programming Language :: Python :: 3',
                   'License :: OSI Approved :: MIT License',
                   'Operating System :: OS Independent',
                   ]
      )
