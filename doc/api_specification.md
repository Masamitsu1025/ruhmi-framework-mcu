## 4.1 Module contents

### 4.1.1 mera module
Mera: Public API for Mera ML compiler stack.

### 4.1.2 mera.deploy module
Mera Deployer classes
mera.deploy.Deployer
alias of MERADeployer
class mera.deploy.MERADeployer(output_dir, overwrite=False)
Bases: _DeployerBase
MERA standard deployer with MERA’s compiler stack:
Create a new deployment project with a given toolchain
Parameters
• output_dir (str) – Output directory relative to cwd where the project should be deployed
• overwrite (bool) – Whether the folder should be wiped before starting a new deployment.
Defaults to false
deploy(model, mera_platform=Platform.SAKURA_2C, build_config={}, target=Target.Simulator,
host_arch=None, mcu_config={}, vela_config={}, **kwargs)
Launches the compilation of a MERA project for a MERA model using the MERA stack.
Parameters
• model (MeraModel) – Model object loaded from mera.ModelLoader
• mera_platform (Platform) – MERA platform architecture enum value
• build_config – MERA build configuration dict
• target (Target) – MERA build target
• host_arch (str) – Host arch to deploy for. If unset, it will pick the current host platform,
provide a value to override the setting.
• mcu_config – Dictionary with user overrides for MCU CCodegen tool. The following
fields are allowed: suffix, weight_location, use_x86
• vela_config – Dictionary with user overrides for MCU Vela tool. The following fields
are allowed: enable_ospi, config, sys_config, accel_config, optimise, memory_mode, verbose_
all.
Returns
The object representing the result of a MERA deployment

### 4.1.3 mera.deploy_project module
Mera Deploy Project utilities.
class mera.deploy_project.Layout(*values)
Bases: Enum
List of possible data layouts
NCHW = 'NCHW'
N batches, Channels, Height, Width.
NHWC = 'NHWC'
N batches, Height, Width, Channels.
class mera.deploy_project.Target(*values)
Bases: Enum
List of possible Mera Target values.
IP = ('IP', False, False)
Target HW accelerator. Valid for arm and x86 architectures.
Interpreter = ('Interpreter', True, True)
Target sw interpretation of the model in floating point. Only valid for x86
InterpreterBf16 = ('InterpreterBf16', True, True)
Target sw interpretation of the model in BF16. Only valid for x86
InterpreterHw = ('InterpreterHw', True, False)
Target sw interpretation of the model. Only valid for x86
InterpreterHwBf16 = ('InterpreterHwBf16', True, True)
Target IP sw interpretation of the model in BF16. Only valid for x86
MCU = ('MCU', False, True)
MERA2Interpreter = ('MERAInterpreter', True, True)
MERAInterpreter = ('MERAInterpreter', True, True)
Quantizer = ('Quantizer', True, True)
Simulator = ('Simulator', True, False)
Target sw simulation of the IP model. Only valid for x86
SimulatorBf16 = ('SimulatorBf16', True, True)
Target sw simulation of the IP BF16 model. Only valid for x86
VerilatorSimulator = ('VerilatorSimulator', True, False)
Target hw emulation of the IP model. Only valid for x86
26 v2.3.2©EDGECORTIX. ALL RIGHTS RESERVED. Proprietary and Confidential Information
mera.deploy_project.is_mera_project(path)
Returns whether a provided path is a MeraProject or not
Parameters
path (str) – Path to check for project existence
Returns
Whether the path belongs to a project
Return type
bool

### 4.1.4 mera.mera_deployment module
Mera Deployment classes
class mera.mera_deployment.DeviceTarget(*values)
Bases: Enum
List of possible MERA runtime devices for running IP deployments.
INTEL_IA420 = ('Intel IA420', 3)
Target device is an Intel IA420 FPGA board.
SAKURA_1 = ('Sakura-1', 1)
Target device is an EdgeCortix’s Sakura-1 ASIC.
SAKURA_2 = ('Sakura-2', 5)
Target device is an EdgeCortix’s Sakura-2 ASIC.
XILINX_U50 = ('AMD Xilinx U50', 2)
Target device is an AMD Xilinx U50 FPGA board.
property code
class mera.mera_deployment.MeraDeployment(plan_loc, target)
Bases: object
get_runner(device_target=DeviceTarget.SAKURA_1, device_ids=None, dynamic_output_list=None)
Prepares the model for running with a given target
Parameters
• device_target (DeviceTarget) – Selects the device run target where the IP deployment
will be run. Only applicable for deployments with target=IP. See DeviceTarget enum for a
detailed list of possible values.
• device_ids (int | List[int]) – When running in a multi card environment, selects
the SAKURA device(s) where the deployment will be run. If unset, MERA will
automatically select any available card in the system. Only applicable in the case device_
target=DeviceTarget.SAKURA_1
• dynamic_output_list (List[str | int]) – Marks certain outputs so that only a dynamic
subset of the data is returned. See special get_output_row() function in MeraModelRunner.
This feature is only supported when running in IP.
Returns
Runner object
Return type
MeraModelRunner
class mera.mera_deployment.MeraInterpreterDeployment(model_loc)
Bases: object
get_runner(profiling_mode=False, config_dict={}, **kwargs)
Prepares the Interpreter for running the model.
Parameters
• profiling_mode (bool) – Enables collection of node execution times.
• config_dict (Dict)
Returns
Runner object
Return type
MeraInterpreterModelRunner
class mera.mera_deployment.MeraInterpreterModelRunner(int_runner, int_cfg)
Bases: ModelRunnerBase
display_profiling_table()
get_num_inputs()
Return type
int
get_num_outputs()
Gets the number of available outputs
Returns
Number of output variables
Return type
int
get_output(output_idx=0)
Returns the output tensor given an output id index. run() needs to be called before get_output()
Parameters
output_idx (int) – Index of output variable to query
Returns
Output tensor values in numpy format
Return type
ndarray
get_output_row(row_idx, output_idx=0)
Parameters
• row_idx (int)
• output_idx (int)
Return type
ndarray
get_outputs()
Returns a list of all output tensors. Equivalent to get_output() from [0, get_num_outputs()]
Returns
List of output tensor values in numpy format
Return type
List[ndarray]
get_outputs_dict()
Return type
Dict[str, ndarray]
get_power_metrics()
Gets the power metrics reported from MERA after a run(). Note power measurement mode might need
to be enable in order to collect and generate such metrics.
Returns
Container with summary analysis of all collected metrics from MERA.
Return type
PowerMetrics
get_runtime_metrics()
Gets the runtime metrics reported from Mera after a run()
Returns
Dictionary of measured metrics
Return type
dict
run()
Runs the model with the specified input data. set_input() needs to be called before run()
Return type
None
set_input(data)
Sets the input data for running
Parameters
data (Dict[str, ndarray]) – Input numpy data tensor or dict of input numpy data tensors
if the model has more than one input. Setting multiple inputs should have the format
{input_name : input_data}
class mera.mera_deployment.MeraInterpreterPrjDeployment(model_loc, prj)
Bases: MeraInterpreterDeployment
class mera.mera_deployment.MeraModelRunner(runner, plan)
Bases: ModelRunnerBase
get_input_handle(name, as_numpy=True, dtype='float32')
Gets the zero-copy handler to the specified model input. :param name: Name of the input. :param
as_numpy: Whether to prepare handle as numpy array. Defaults to true. :param dtype: Viewer data type.
Returns
Input data handler.
Parameters
• name (str)
• as_numpy (bool)
• dtype (str)
get_input_names()
Return type
List[str]
get_num_outputs()
Gets the number of available outputs
Returns
Number of output variables
Return type
int
get_output(output_idx=0)
Returns the output tensor given an output id index. run() needs to be called before get_output()
Parameters
output_idx (int) – Index of output variable to query
Returns
Output tensor values in numpy format
Return type
ndarray
get_output_handle(name, as_numpy=True, dtype='float32')
Gets the zero-copy handler to the specified model output. :param name: Name of the output. :param
as_numpy: Whether to prepare handle as numpy array. Defaults to true. :param dtype: Viewer data type.
Returns
Output data handler.
Parameters
• name (str)
• as_numpy (bool)
• dtype (str)
get_output_names()
Return type
List[str]
get_output_row(row_idx, output_idx=0)
Parameters
• row_idx (int)
• output_idx (int)
Return type
ndarray
get_outputs()
Returns a list of all output tensors. Equivalent to get_output() from [0, get_num_outputs()]
Returns
List of output tensor values in numpy format
Return type
List[ndarray]
get_outputs_dict()
Return type
Dict[str, ndarray]
get_power_metrics()
Gets the power metrics reported from MERA after a run(). Note power measurement mode might need
to be enable in order to collect and generate such metrics.
Returns
Container with summary analysis of all collected metrics from MERA.
Return type
PowerMetrics
get_runtime_metrics()
Gets the runtime metrics reported from Mera after a run()
Returns
Dictionary of measured metrics
Return type
dict
run()
Runs the model with the specified input data. set_input() needs to be called before run()
Return type
None
set_input(data)
Sets the input data for running
Parameters
data (ndarray | Dict[str, ndarray] | List[ndarray]) – Input numpy data tensor
or dict of input numpy data tensors if the model has more than one input. Setting multiple
inputs should have the format {input_name : input_data}
set_named_input(name, data)
Gets the zero-copy numpy handler and copies data to the device. :param name: Name of the input.
Parameters
• name (str)
• data (ndarray)
class mera.mera_deployment.MeraPrjDeployment(plan_loc, prj, target)
Bases: MeraDeployment
class mera.mera_deployment.MeraTvmModelRunner(rt_mod)
Bases: ModelRunnerBase
get_num_outputs()
Gets the number of available outputs
Returns
Number of output variables
Return type
int
get_output(output_idx=0)
Returns the output tensor given an output id index. run() needs to be called before get_output()
Parameters
output_idx (int) – Index of output variable to query
Returns
Output tensor values in numpy format
Return type
ndarray
get_outputs()
Returns a list of all output tensors. Equivalent to get_output() from [0, get_num_outputs()]
Returns
List of output tensor values in numpy format
Return type
List[ndarray]
get_power_metrics()
Gets the power metrics reported from MERA after a run(). Note power measurement mode might need
to be enable in order to collect and generate such metrics.
Returns
Container with summary analysis of all collected metrics from MERA.
Return type
PowerMetrics
get_runtime_metrics()
Gets the runtime metrics reported from Mera after a run()
Returns
Dictionary of measured metrics
Return type
dict
run()
Runs the model with the specified input data. set_input() needs to be called before run()
Return type
None
set_input(data)
Sets the input data for running
Parameters
data (ndarray | Dict[str, ndarray] | List[ndarray]) – Input numpy data tensor
or dict of input numpy data tensors if the model has more than one input. Setting multiple
inputs should have the format {input_name : input_data}
class mera.mera_deployment.ModelRunnerBase
Bases: object
API for runtime inference of a model.
abstractmethod get_num_outputs()
Gets the number of available outputs
Returns
Number of output variables
Return type
int
abstractmethod get_output(output_idx=0)
Returns the output tensor given an output id index. run() needs to be called before get_output()
Parameters
output_idx (int) – Index of output variable to query
Returns
Output tensor values in numpy format
Return type
ndarray
abstractmethod get_outputs()
Returns a list of all output tensors. Equivalent to get_output() from [0, get_num_outputs()]
Returns
List of output tensor values in numpy format
Return type
List[ndarray]
abstractmethod get_power_metrics()
Gets the power metrics reported from MERA after a run(). Note power measurement mode might need
to be enable in order to collect and generate such metrics.
Returns
Container with summary analysis of all collected metrics from MERA.
Return type
PowerMetrics
abstractmethod get_runtime_metrics()
Gets the runtime metrics reported from Mera after a run()
Returns
Dictionary of measured metrics
Return type
dict
abstractmethod run()
Runs the model with the specified input data. set_input() needs to be called before run()
Return type
None
abstractmethod set_input(data)
Sets the input data for running
Parameters
data (ndarray | Dict[str, ndarray] | List[ndarray]) – Input numpy data tensor
or dict of input numpy data tensors if the model has more than one input. Setting multiple
inputs should have the format {input_name : input_data}
mera.mera_deployment.load_mera_deployment(path, target=None)
Loads an already built deployment from a directory
Parameters
• path (str) – Directory of a Mera deployment project or full directory of built mera results
• target (Target) – If there are multiple targets built in the mera project selects which one.
Optional if not loading a project or if there is a single target built.
Returns
Reference to deployment object

### 4.1.5 mera.mera_model module
Mera Model classes.
class mera.mera_model.Mera2ModelFused(prj, fused_path, mera_models, share_input, fused_model_name)
Bases: MeraModel
class mera.mera_model.Mera2ModelQuantized(prj, model_name, model_path)
Bases: MeraModel
MeraModel class of a model quantized with MERA2 tools.
class mera.mera_model.MeraModel(prj, model_name, model_path, use_prequantize_input=False,
save_model=False, model_load_idx=None)
Bases: object
Base class representing a ML model compatible with MERA deployment project.
get_input_shape(input_name=None)
Utility class to query the shape of an input variable of the model
Parameters
input_name (str) – Specifies which input to get the shape from. If unset, assumes there is
only one input.
Returns
A tuple with 4 items representing the shape of the input variable in the model.
Return type
Tuple[int]
property input_desc
class mera.mera_model.MeraModelExecutorch(prj, model_name, model_path)
Bases: MeraModel
Specialization of MeraModel for a Executorch/EXIR ML model.
class mera.mera_model.MeraModelOnnx(prj, model_name, model_path, batch_num, shape_mapping,
model_info, model_load_idx)
Bases: MeraModel
Specialization of MeraModel for a ONNX ML model.
class mera.mera_model.MeraModelTflite(prj, model_name, model_path, use_prequantize_input,
model_load_idx=None)
Bases: MeraModel
Specialization of MeraModel for a TFLite ML model.
34 v2.3.2©EDGECORTIX. ALL RIGHTS RESERVED. Proprietary and Confidential Information
class mera.mera_model.ModelLoader(deployer=None)
Bases: object
Utility class for loading and converting ML models into models compatible with MERA
Parameters
deployer (mera.deploy.Deployer) – Reference to a MERA deployer class, if None is provided,
information about the model will not be added to the deployment project.
from_executorch(model_path, model_name=None)
Converts a PyTorch model in Executorch/EXIR format (.pte) into a compatible model for MERA.
Parameters
• model_path (str) – Path to the PyTorch model file in ExecuTorch format (.pte)
• model_name (str) – Display name of the model being deployed. Will default to the stem
name of the model file if not provided.
Returns
The input model compatible with MERA.
Return type
MeraModelExecutorch
from_onnx(model_path, model_name=None, layout=Layout.NHWC, batch_num=1, shape_mapping={},
model_info={}, model_load_idx=None)
Converts a ONNX model into a compatible model for MERA. NOTE this loader is best optimised for float
models using op_set=12
Parameters
• model_path (str) – Path to the ONNX model file.
• model_name (str) – Display name of the model being deployed. Will default to the stem
name of the model file if not provided.
• layout (Layout) – Data layout of the model being loaded. Defaults to NHWC layout
• batch_num (int) – If the model contains symbolic batch numbers, loads it resolving its
value to the parameter provided. Defaults to 1.
• shape_mapping (Dict[str, int]) – If the model contains symbolic shapes, provides
their static mapping.
• model_info (Dict) – An optional dictionary with model’s metadata or other hyperparameters.
• model_load_idx (int) – An optional id to be used with model fusion, in order to identify
the different models to be fused.
Returns
The input model compatible with MERA.
Return type
MeraModelOnnx
from_pytorch(model_path, input_desc, model_name=None, layout=Layout.NHWC,
use_prequantize_input=False)
<<Deprecated>> Converts a PyTorch model in TorchScript format into a compatible model for MERA.
Parameters
• model_path (str) – Path to the PyTorch model file in TorchScript format
• input_desc (Dict[str, tuple]) – Map of input names and their dimensions and types.
Expects a format of {input_name : (input_size, input_type)}
• model_name (str) – Display name of the model being deployed. Will default to the stem
name of the model file if not provided.
• layout (Layout) – Data layout of the model being loaded. Defaults to NHWC layout
• use_prequantize_input (bool) – Whether input is provided prequantized, or not. Defaults
to False
Returns
The input model compatible with MERA.
Return type
MeraModelPytorch
from_quantized_mera(model_path, model_name=None, use_legacy=False)
Converts a previously quantized MERA model into a compatible deployable model.
Parameters
• model_path (str) – Path to the MERA model file
• model_name (str) – Display name of the model being deployed. Will default to the stem
name of the model file if not provided.
• use_legacy (bool) – Whether to use older MERA v1 model loader. Use only in the case
of legacy quantizer.
Returns
The input model compatible with MERA.
from_tflite(model_path, model_name=None, use_prequantize_input=False, model_load_idx=None)
Converts a tensorflow model in TFLite format into a compatible model for MERA.
Parameters
• model_path (str) – Path to the tensorflow model file in TFLite format
• model_name (str) – Display name of the model being deployed. Will default to the stem
name of the model file if not provided.
• use_prequantize_input (bool) – Whether input is provided prequantized, or not. Defaults
to False
• model_load_idx (int) – An optional id to be used with model fusion, in order to identify
the different models to be fused.
Returns
The input model compatible with MERA.
Return type
MeraModelTflite
fuse_models(mera_models, share_input=False, fused_model_name=None)
Fusing multiple MERA models into a single model for compilation and deployment.
This is especially useful for fully utilizing the compute resources of a large platform. The inputs of
the fused model are the concatenation of the inputs of the models to be fused. Similarly, the outputs
of the fused model are the concatenation of the outputs of the models to be fused. For example, let’s
suppose mera_models has two models, m1 and m2, then for the fused model, the inputs are [m1 inputs,
m2 inputs] and the outputs are [m1 outputs, m2 outputs]. When each model in mera_models has one
input and share_input is True, the fused model has one input.
Parameters
• mera_models (Tuple[MeraModel]) – List of MERA models to be fused.
• share_input (bool) – Whether the models share input or not.
• fused_model_name (str) – Overrides the autogenerated name for the fused model result.
Returns
The fused model.
Return type
MeraModelFused

### 4.1.6 mera.mera_platform module
MERA platform selection
class mera.mera_platform.AccelKind(*values)
Bases: Enum
CPU = 'CPU'
DNA = 'DNA'
GPU = 'GPU'
MCU = 'MCU'
class mera.mera_platform.Platform(*values)
Bases: Enum
List of all valid MERA platforms
ALT1 = ('ALT1', AccelKind.MCU)
ALT2 = ('ALT2', AccelKind.MCU)
DNAA400L0001 = 'DNAA400L0001'
DNAA600L0001 = 'DNAA600L0001'
DNAA600L0002 = 'DNAA600L0002'
DNAF10032x2 = 'DNAF10032x2'
DNAF100L0001 = 'DNAF100L0001'
DNAF100L0002 = 'DNAF100L0002'
DNAF100L0003 = 'DNAF100L0003'
DNAF132S0001 = 'DNAF132S0001'
DNAF200L0001 = 'DNAF200L0001'
DNAF200L0002 = 'DNAF200L0002'
DNAF200L0003 = 'DNAF200L0003'
DNAF232S0001 = 'DNAF232S0001'
v2.3.2©EDGECORTIX. ALL RIGHTS RESERVED. Proprietary and Confidential Information 37
DNAF232S0002 = 'DNAF232S0002'
DNAF300L0001 = 'DNAF300L0001'
DNAF632L0001 = 'DNAF632L0001'
DNAF632L0002 = 'DNAF632L0002'
DNAF632L0003 = 'DNAF632L0003'
MCU_CPU = ('ALT1', AccelKind.MCU)
MCU_ETHOS = ('ALT2', AccelKind.MCU)
SAKURA_1 = 'DNAA600L0002'
SAKURA_2 = 'DNAA600L0003'
SAKURA_2C = 'DNAA600L0003'
SAKURA_I = 'DNAA600L0002'
SAKURA_II = 'DNAA600L0003'
property accelerator_kind
property platform_name

### 4.1.7 mera.version module
mera.version.get_mera2_rt_version()
“return: The version string for mera2-runtime
Return type
str
mera.version.get_mera_dna_version()
Gets the version string for libmeradna
Returns
Summary of libmeradna version
Return type
str
mera.version.get_mera_tvm_version()
Gets the version string for mera-tvm module
Returns
mera-tvm version
Return type
str
mera.version.get_mera_version()
Gets the version string for Mera
Returns
Version string for Mera
Return type
str
mera.version.get_versions()
Return a summary of all installed modules on the Mera environment
Returns
List of all module’s versions.
Return type
str

### 4.1.8 mera.mera_quantizer module
Mera Quantizer classes
class mera.mera_quantizer.Quantizer(deployer, model,
quantizer_config=<mera.quantizer.quantizer_config.QuantizerConfig
object>, mera_platform=Platform.SAKURA_2C, **kwargs)
Bases: object
Class with API to quantize models using MERA
Creates a class for creating MERA quantized models.
Parameters
• deployer – Instance of deployment project created with mera.Deployer.
• model – Model to be quantized loaded with mera.ModelLoader.
• quantizer_config (QuantizerConfig) – Instance of quantizer configuration with user
defined options.
• mera_platform (Platform) – Platform to be used for quantizer deployment.
apply_smoothquant(alpha=0.5, autotune=True)
Parameters
• alpha (float)
• autotune (bool)
calibrate(calibration_data)
Feeds a series of realistic input data samples in order to be able to compute accurate internal ranges. MERA
will collect the information from the execution of these data samples and compute the quantization domains
as determined by the user configuration. It is recommended to use a big enough dataset of realistic samples
in order to obtain the best quantization accuracy results.
Parameters
calibration_data (List[Dict[str, ndarray]]) – List of dictionaries with the format
{‘input_name’ : ‘np_array’} containing the different data samples.
evaluate_quality(evaluation_data, display_table=True)
Measures the quantization quality of a transformed model with a given evaluation data. This should be
some realistic data sample(s) ideally different from the calibration dataset. In order to measure quality the
user must have called quantize() method first.
Parameters
• evaluation_data (List[Dict[str, ndarray]]) – List of dictionaries with the format
{‘input_name’ : ‘np_array’} containing the different data samples.
• display_table (bool) – Whether to display quality metrics to stdout or not.
v2.3.2©EDGECORTIX. ALL RIGHTS RESERVED. Proprietary and Confidential Information 39
Returns
List of quality metrics container.
get_report(model_id)
Extracts all information about the quantization process as a dictionary that can be saved for debugging.
Parameters
model_id – Identifier to be used for this document.
quantize()
Uses the data gathered from the calibrate() method and creates a transformed model based on the quantizer
configuration.
reset()
Resets all the internal observed metrics of the quantizer as well as any existing qtz transformed model.
save_to(dst_path)
Saves the transformed model to file. Must have called quantize() first. :param dst_path: Destination path
where the model will be saved.
mera.mera_quantizer.get_input_desc(mera_model_path)
Retrieve the input description of a MERA quantized model generated with MERA2.
Parameters
mera_model_path – Path to .mera model file.
Returns
Dict with info about the model’s inputs.
Return type
InputDescriptionContainer

### 4.1.9 mera.quantizer module
MERA Quantizer Configuration classes.
class mera.quantizer.quantizer_config.LayerConfig(conv_act, conv_weights, mm_act, mm_weights)
Bases: object
Set of quantization configurations to be applied for a Layer in the model
Parameters
• conv_act (OperatorConfig)
• conv_weights (OperatorConfig)
• mm_act (OperatorConfig)
• mm_weights (OperatorConfig)
property conv_act: OperatorConfig
property conv_weights: OperatorConfig
property mm_act: OperatorConfig
property mm_weights: OperatorConfig
class mera.quantizer.quantizer_config.ObserverClass(*values)
Bases: Enum
HISTOGRAM = 'HISTOGRAM'
An optimised <min,max> is calculated based on the distribution of the calibration data using a histogram.
Can only be used PER_TENSOR.
MAX_ABS = 'MAX_ABS'
Will get the quantization range as <-max(abs),max(abs)> of the calibration data.
MIN_MAX = 'MIN_MAX'
Will get the quantization range as <min,max> based on the whole calibration data.
class mera.quantizer.quantizer_config.OperatorConfig(qtype, qscheme, qmode, qtarget, observer,
**kwargs)
Bases: object
Set of quantizer configurations to be applied to an operator.
Parameters
• qtype (QType)
• qscheme (QScheme)
• qmode (QMode)
• qtarget (QTarget)
• observer (ObserverClass)
property observer: ObserverClass
property qmode: QMode
property qscheme: QScheme
property qtarget: QTarget
property qtype: QType
set_options(histogram_n_bins=None, histogram_obs_upsample_rate=None, per_channel_limit=None,
per_channel_grp_size=None, use_symmetric_range=None)
Sets advanced quantization options for this operator.
Parameters
• histogram_n_bins (int) – When using histogram observer, overrides default number of
bins used.
• histogram_upsample_rate – When using histogram observer, overrides default upsample
rate for histogram aggregations.
• per_channel_limit (int) – Architecture limitation to mark the maximum number of
channels of a tensor possible where PER_CHANNEL quantization can still be done. Any
operation above this limit will switch to use PER_CHANNEL_GROUP instead.
• per_channel_grp_size (int) – When using PER_CHANNEL_GROUP, specifies the
max size of q_params that will group all the channels in a tensor.
• use_symmetric_range (bool) – Reduces the range of quantization so that values are set
in <-MaxVal,MaxVal>. e.g. [-127,127] for int8 type. Only valid for the case of signed
quantization.
• histogram_obs_upsample_rate (int)
v2.3.2©EDGECORTIX. ALL RIGHTS RESERVED. Proprietary and Confidential Information 41
class mera.quantizer.quantizer_config.QMode(*values)
Bases: Enum
PER_CHANNEL = 'PER_CHANNEL'
A different set of <scale,zero_point> for each of the tensor’s channels.
PER_CHANNEL_GROUP = 'PER_CHANNEL_GROUP'
A different set of <scale,zero_point> for each group of several tensor’s channels.
PER_TENSOR = 'PER_TENSOR'
Single set of <scale,zero_point> for the whole tensor.
class mera.quantizer.quantizer_config.QScheme(*values)
Bases: Enum
AFFINE = 'AFFINE'
Quantization range adjusted to observed <min,max> from data
SYMMETRIC = 'SYMMETRIC'
Quantization range centered around real value 0.
class mera.quantizer.quantizer_config.QTarget(*values)
Bases: Enum
DATA = 'DATA'
Tensor representing the activated data of a quantizable operation.
WEIGHT = 'WEIGHT'
Tensor are the weights of a quantizable operation.
class mera.quantizer.quantizer_config.QType(*values)
Bases: Enum
BF16 = 'BF16'
Unquantized BrainFloat16 type.
S7 = 'S7'
7-bit signed, ranged [-64, 63]
S8 = 'S8'
8-bit signed, ranged [-128, 127]
U7 = 'U7'
7-bit unsigned, ranged [0, 127]
U8 = 'U8'
8-bit unsigned, ranged [0, 255]
class mera.quantizer.quantizer_config.QuantizerConfig(global_cfg, flow_version=1)
Bases: object
Class representing the configuration of the MERA quantizer.
Parameters
• global_cfg (LayerConfig)
• flow_version (int)
to_dict()
property transform_cfg: TransformConfig
class mera.quantizer.quantizer_config.QuantizerConfigPresets
Bases: object
ALT = <mera.quantizer.quantizer_config.QuantizerConfig object>
DEFAULT = <mera.quantizer.quantizer_config.QuantizerConfig object>
Sample base configuration for DNA quantizations.
DNA_SAKURA_II = <mera.quantizer.quantizer_config.QuantizerConfig object>
MCU = <mera.quantizer.quantizer_config.QuantizerConfig object>
Sample base configuration for MCU quantizations.
class mera.quantizer.quantizer_config.TransformConfig
Bases: object
Class representing options for transformation of model into quantized MERA model.
property fuse_i8_concat_domains: bool
property glu_bf16_outlier_threshold: float
property map_silu_to_hswish: bool
property use_bf16_for_small_ch_conv: bool
Wrapper class for quantizer quality objects
class mera.quantizer.quality.QuantizationQuality(data, out_names)
Bases: object
Container class that holds different quality metrics of a quantized tensor.
node_summary()
Returns a metric summary of the intermediate nodes of the model.
out_summary()
Returns a metric summary of the outputs of the model.
to_table(extra_debug_info=False)
Returns a tabulated table representation of the data
Parameters
extra_debug_info (bool)
