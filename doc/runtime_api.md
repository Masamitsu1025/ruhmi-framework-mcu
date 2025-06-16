Guide to the generated C source code
************************************

After processing a model with the compiler, you will find several files on your deployment directory. This include some deploying artifacts generated during compilation that are worth to be kept around for debugging purposes.

The most important output that RHUMI framework generates is found under the directory **<deployment_directory>build/MCU/compilation/src**. This directory contains the model converted into a set of C99 source code files.

### an example of the folder structure
```
 models_int8/  
  ├── ad_medium_int8.tflite
       ├── build  
            ├── MCU  
                ├── compilation  
                   ├── mera.plan  
                   ├── src     # compilation results: C source code and C++ testing support code # HAL entry example  
                      ├── CMakeLists.txt  
                      ├── compare.cpp  
                      ├── compute_sub_0000.c # CPU subgraph generated C source code  
                      ├── compute_sub_0000.h  
                      ├── ...  
                      ├── ethosu_common.h  
                      ├── hal_entry.c  
                      ├── kernel_library_int.c # kernel library if CPU subgraphs are present  
                      ├──  ...  
                      ├── model.c  
                      ├── model.h  
                      ├── model_io_data.c  
                      ├── model_io_data.h  
                      ├── python_bindings.cpp  
                      ├── sub_0001_command_stream.c # Ethos-U55 subgraph generated C source code  
                      ├── sub_0001_command_stream.h  
                      ├── sub_0001_invoke.c  
                      ├── sub_0001_invoke.h  
                      ├──  ...  
```

Runtime API - MPU only deployment
=================================

When a model is converted into source code with MERA compiler without Ethos-U support, all the operators in the model being deployed will be prepared to be run on CPU/MCU only. In this case, the generated code will refer to a single subgraph **compute_sub_0000<suffix>**, by default, when no suffix is provided, the name of the header that need to included on your application entry point is **compute_sub_0000.h**.

This header provides the declaration of a C function that if called it will run the model with the provided inputs and write the results on the provided output buffers:

.. code-block:: c

   enum BufferSize_sub_0000 {
     kBufferSize_sub_0000 = <intermediate_buffers_size>
   };

   void compute_sub_0000(
     // buffer for intermediate results
     uint8_t* main_storage, // should provide at least <intermediate_buffers_size> bytes of storage

     // inputs
     const int8_t <input_name>[xxx], // 1,224,224,3

     // outputs
     int8_t <output_name>[xxx]  // 1,1000
   );

It provides to the user the possibility of providing a buffer to hold intermediate outputs of the model. And this size if provided in compilation time as the value **kBufferSize_sub_0000** so the user can use this size to allocate the buffer on the stack, the heap or a custom data section.


Runtime API - MPU + Ethos-U deployment
======================================

If Ethos-U support is enabled during conversion into source code with MERA compiler then an arbitrary amount of subgraphs for either CPU or Ethos-U will be generated. Each of these subgraphs will correspond to generated C functions to run the corresponding section of the model on CPU or Ethos. Each function call will get its inputs from previous outputs of other subgraphs and write its outputs on buffers that are designated to became again inputs to other functions and so on. To make easier for the user to invoke these models where CPU and NPU are involved, the generated code will automate this process and provide a single function that will orchestrate the calls to the different computation units named **void RunModel(bool clean_outputs)** and helpers to access to each of the input and output areas at **model level** not per subgraph level. The runtime API header when Ethos-U is enabled can be found on a file named **model.h** under the same directory **<deployment_directory>/build/MCU/compilation/src**.

For example, after enabling Ethos-U support for a model with two inputs and three outputs MERA provides the next runtime API:

.. code-block:: c

   // invoke the whole model
   void RunModel(bool clean_outputs);

     // Model input pointers
   float* GetModelInputPtr_input0();
   float* GetModelInputPtr_input1();

     // Model output pointers
   float* GetModelOutputPtr_out0();
   float* GetModelOutputPtr_out1();
   float* GetModelOutputPtr_out2();

The function **GetModelInputPtr_input0** provides access to the buffer where the user can write the first input of the model and **GetModelInputPtr_input1** gives us the pointer where the user can copy the second input.

To run the model and all the CPU or NPU units needed to be invoked to do inference with the deployed model, the user should invoke to the **RunModel()** function. The parameter **clean_outputs** should be used only for debugging purposes because it will set to zero all the output buffers used by an NPU unit before invoking it. Recommended value for the parameter **clean_outputs** is **false**, as it will not incur into extra time expend on clearing these buffers.

Testing helpers for x86
=======================

In order to test the generated C99 code without deploying it to the board there is a configuration option that enables the generation of all these helpers. This option can be enabled as below:


.. code-block:: python
   with mera.Deployer(str(deploy_dir), overwrite=True) as deployer:
       mera_model = mera.ModelLoader(deployer).from_quantized_mera(model_path)
       mcu_config = {}
       mcu_config['use_x86'] = True # generates x86 helpers

       deployer.deploy(mera_model,
                       mera_platform=Platform.MCU_CPU,
                       target=Target.MCU,
                       vela_config=vela_config,
                       mcu_config=mcu_config,
                       enable_ref_data=enable_ref_data)

With this option enabled we will see some extra C++ files generated:

.. code-block::
   ������ deployment_directory
   ��   ������ build
   ��   ��   ������ MCU
   ��   ��       ������ compilation
   ��   ��       ��   ������ src
   ��   ��       ��   ��   ������ CMakeLists.txt
   ��   ��       ��   ��   ������ cmsis-windows.patch
   ��   ��       ��   ��   ������ compare.cpp
   ��   ��       ��   ��   ������ hal_entry.c
   ��   ��       ��   ��   ������ python_bindings.cpp

* **CMakeLists.txt** CMake project that compiles CMSIS-NN, the generated C99 code and Python bindings into a shared library (Python module).
* **cmsis-windows.patch** The CMake project will pull dependencies as CMSIS-NN. In order to be able to compile CMSIS-NN on Windows this patch is necessary.
* **compare.cpp** Small C++ application generated for testing the generated C99 code on x86.
* **hal_entry.c** Auto-generated example of a possible entry point on Renesas e? studio project to get the user a starting point on how to run the model. This generated code intended to be used as a reference by the user.
* **python_bindings.cpp** Contains auto-generated PyBind11 bindings for the C99 generated code.