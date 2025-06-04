from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "filter",                      # Назва модуля Python
        ["filter.cpp"],                # Твій C++ файл
        include_dirs=[pybind11.get_include()],
        language="c++",
        extra_compile_args=["-O3", "-std=c++11"],  # Оптимізація, стандарт C++
    ),
]

setup(
    name="filter",
    ext_modules=ext_modules,
    zip_safe=False,
)
