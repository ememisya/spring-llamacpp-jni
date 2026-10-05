### Building

```
mvn install -DcompileLlamaCpp=true -DcompileJNI=true
```

### Cleaning

```
mvn clean -Dall=true
```

When "all" is set to true, native builds are also removed to skip front-end
(node_modules) include *skipFrontend* switch

### Running Locally

To run the back-end simply execute:

```
mvn spring-boot:run
```

To run the front-end simply execute:

```
npm run dev
```

### Testing

```
mvn verify
```

to skip front-end tests include *skipFrontend* switch

#### Common issues

```
   No CMAKE_CUDA_COMPILER could be found.
   Tell CMake where to find the compiler by setting either the environment
   variable "CUDACXX" or the CMake cache entry CMAKE_CUDA_COMPILER to the full
   path to the compiler, or to the compiler name if it is in the PATH.
```

Simply locate the path for your "nvcc" executable and rerun the build after
setting "CUDACXX".

```bash
   export CUDACXX=/usr/local/cuda-13.0/bin/nvcc
```

--------------------------


