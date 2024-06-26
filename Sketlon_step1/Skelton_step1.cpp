#include "Skelton_step1.h"


static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_SPRINTF(	out_data->return_msg, 
				"%s, v%d.%d\r%s",
				NAME, 
				MAJOR_VERSION, 
				MINOR_VERSION, 
				DESCRIPTION);

	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (
	PF_InData		*in_dataP,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;

	out_data->my_version	= PF_VERSION(	MAJOR_VERSION, 
											MINOR_VERSION,
											BUG_VERSION, 
											STAGE_VERSION, 
											BUILD_VERSION);
	
	out_data->out_flags		=	PF_OutFlag_PIX_INDEPENDENT	|
								PF_OutFlag_DEEP_COLOR_AWARE |
								PF_OutFlag_NON_PARAM_VARY;

	out_data->out_flags2	=	PF_OutFlag2_FLOAT_COLOR_AWARE	|
								PF_OutFlag2_SUPPORTS_SMART_RENDER	|
								PF_OutFlag2_SUPPORTS_THREADED_RENDERING;

	return err;
}

static PF_Err 
ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;
	
	AEFX_CLR_STRUCT(def);
	
	PF_ADD_FLOAT_SLIDERX("Noise variation", 
						FILTER_NOISE_MIN,
						FILTER_NOISE_MAX,
						SLIDER_MIN,
						SLIDER_MAX,
						FILTER_NOISE_DFLT,
						SLIDER_PRECISION,
						DISPLAY_FLAGS,
						0,
						SLIDER_DISK_ID);
	
	out_data->num_params = PARAM_NUM_PARAMS;

	return err;
}

static PF_Err 
FilterImage8 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	PF_FpLong	tempF		= 0;
					
	if (niP){
		tempF 	= rand() % PF_MAX_CHAN8;
		tempF	*= (niP->valF / SLIDER_MAX);

		outP->alpha		=	inP->alpha;
		outP->red		=	MIN(PF_MAX_CHAN8, inP->red + (A_u_char) tempF);
		outP->green		=	MIN(PF_MAX_CHAN8, inP->green + (A_u_char) tempF);
		outP->blue		=	MIN(PF_MAX_CHAN8, inP->blue + (A_u_char) tempF);
	}
	return err;
}

static PF_Err 
FilterImage16 (
	 void		*refcon,
	 A_long		xL, 
	 A_long		yL, 
	 PF_Pixel16	*inP, 
	 PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	PF_FpLong	tempF	= 0;
	
	if (niP){
		tempF 	= rand() % PF_MAX_CHAN16;
		tempF	*= (niP->valF / SLIDER_MAX);
		
		outP->alpha		=	inP->alpha;
		outP->red		=	MIN(PF_MAX_CHAN16, inP->red + (A_u_short) tempF);
		outP->green		=	MIN(PF_MAX_CHAN16, inP->green + (A_u_short) tempF);
		outP->blue		=	MIN(PF_MAX_CHAN16, inP->blue + (A_u_short) tempF);
	}
	return err;
}

static PF_Err 
FilterImage32 (
	 void			*refcon,
	 A_long			xL, 
	 A_long			yL, 
	 PF_PixelFloat	*inP, 
	 PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	PF_FpShort	tempF	= 0;
	
	if (niP){
		tempF 	= (PF_FpShort)(rand() % PF_MAX_CHAN16);
		tempF	*= (PF_FpShort)(niP->valF / (SLIDER_MAX * PF_MAX_CHAN16));
		
		outP->alpha		=	inP->alpha;
		outP->red		=	(inP->red	+ tempF);
		outP->green		=	(inP->green	+ tempF);
		outP->blue		=	(inP->blue	+ tempF);
	}
	return err;
}
 
static PF_Err 
Render ( 
	PF_InData		*in_dataP,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;

	ParamInfo			niP;
	A_long				linesL	= 0;
	
	AEFX_CLR_STRUCT(niP);
	
	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
	niP.valF 	= params[PARAM_SLIDER]->u.fs_d.value;

	if(params[PARAM_SLIDER]->u.fs_d.value != 0.0) {

		AEFX_SuiteScoper<PF_Iterate8Suite2> iterate8Suite = 
			AEFX_SuiteScoper<PF_Iterate8Suite2>(in_dataP,
												kPFIterate8Suite,
												kPFIterate8SuiteVersion2,
												out_data);

		iterate8Suite->iterate(	in_dataP,
								0,								// progress base
								linesL,							// progress final
								&params[PARAM_INPUT]->u.ld,		// src 
								NULL,							// area - null for all pixels
								(void*)&niP,					// refcon - your custom data pointer
								FilterImage8,					// pixel function pointer
								output);						// dest
	} else {

		AEFX_SuiteScoper<PF_WorldTransformSuite1> worldTransformSuite = 
			AEFX_SuiteScoper<PF_WorldTransformSuite1>(	in_dataP,
														kPFWorldTransformSuite,
														kPFWorldTransformSuiteVersion1,
														out_data);

		worldTransformSuite->copy(	in_dataP->effect_ref,			// This effect ref (unique id)
									&params[PARAM_INPUT]->u.ld,		// Source
									output,							// Dest
									NULL,							// Source rect - null for all pixels
									NULL);							// Dest rect - null for all pixels
	}
	return err;
}

static PF_Err
PreRender(
	PF_InData			*in_dataP,
	PF_OutData			*out_dataP,
	PF_PreRenderExtra	*extraP)
{
	PF_Err err = PF_Err_NONE;
	PF_ParamDef noise_param;
	PF_RenderRequest req = extraP->input->output_request;
	PF_CheckoutResult in_result;
	
	AEFX_CLR_STRUCT(noise_param);

	AEFX_SuiteScoper<PF_HandleSuite1> handleSuite = AEFX_SuiteScoper<PF_HandleSuite1>(	in_dataP,
																						kPFHandleSuite,
																						kPFHandleSuiteVersion1,
																						out_dataP);

	PF_Handle infoH	= handleSuite->host_new_handle(sizeof(ParamInfo));
	
	if (infoH){

		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(handleSuite->host_lock_handle(infoH));
		
		if (infoP){

			extraP->output->pre_render_data = infoH;
			
			ERR(PF_CHECKOUT_PARAM(	in_dataP, 
									PARAM_SLIDER, 
									in_dataP->current_time, 
									in_dataP->time_step, 
									in_dataP->time_scale, 
									&noise_param));
			
			if (!err){
				infoP->valF = noise_param.u.fs_d.value;
			}
			
			ERR(extraP->cb->checkout_layer(	in_dataP->effect_ref,
											PARAM_INPUT,
											PARAM_INPUT,
											&req,
											in_dataP->current_time,
											in_dataP->time_step,
											in_dataP->time_scale,
											&in_result));
			
			UnionLRect(&in_result.result_rect, 		&extraP->output->result_rect);
			UnionLRect(&in_result.max_result_rect, 	&extraP->output->max_result_rect);		
			handleSuite->host_unlock_handle(infoH);
		}
	} else {
		err = PF_Err_OUT_OF_MEMORY;
	}
	return err;
}

static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extraP)
{
	
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;
	
	PF_EffectWorld	*input_worldP	= NULL, 
					*output_worldP  = NULL;

	AEFX_SuiteScoper<PF_HandleSuite1> handleSuite = AEFX_SuiteScoper<PF_HandleSuite1>(	in_data,
																						kPFHandleSuite,
																						kPFHandleSuiteVersion1,
																						out_data);
	
	ParamInfo	*infoP = reinterpret_cast<ParamInfo*>(handleSuite->host_lock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data)));
	
	if (infoP){
		ERR((extraP->cb->checkout_layer_pixels(	in_data->effect_ref, PARAM_INPUT, &input_worldP)));
		ERR(extraP->cb->checkout_output(in_data->effect_ref, &output_worldP));
		
		PF_PixelFormat		format	=	PF_PixelFormat_INVALID;
		
		AEFX_SuiteScoper<PF_WorldSuite2> wsP = AEFX_SuiteScoper<PF_WorldSuite2>(in_data,
																				kPFWorldSuite,
																				kPFWorldSuiteVersion2,
																				out_data);
		
		if (infoP->valF == 0.0) {
			err = PF_COPY(input_worldP, output_worldP, NULL, NULL);
		} else {
			ERR(wsP->PF_GetPixelFormat(input_worldP, &format));
			
			if (!err){

				AEFX_SuiteScoper<PF_iterateFloatSuite2> iterateFloatSuite =
					AEFX_SuiteScoper<PF_iterateFloatSuite2>(in_data,
															kPFIterateFloatSuite,
															kPFIterateFloatSuiteVersion2,
															out_data);
				AEFX_SuiteScoper<PF_iterate16Suite2> iterate16Suite =
					AEFX_SuiteScoper<PF_iterate16Suite2>(	in_data,
															kPFIterate16Suite,
															kPFIterate16SuiteVersion2,
															out_data);

				AEFX_SuiteScoper<PF_Iterate8Suite2> iterate8Suite =
					AEFX_SuiteScoper<PF_Iterate8Suite2>(	in_data,
															kPFIterate8Suite,
															kPFIterate8SuiteVersion2,
															out_data);
				switch (format) {
					
					case PF_PixelFormat_ARGB128:
		
						iterateFloatSuite->iterate(	in_data,
													0,
													output_worldP->height,
													input_worldP,
													NULL,
													(void*)infoP,
													FilterImage32,
													output_worldP);
						break;
						
					case PF_PixelFormat_ARGB64:
										
						iterate16Suite->iterate(in_data,
												0,
												output_worldP->height,
												input_worldP,
												NULL,
												(void*)infoP,
												FilterImage16,
												output_worldP);
						break;
						
					case PF_PixelFormat_ARGB32:
			
						iterate8Suite->iterate(	in_data,
												0,
												output_worldP->height,
												input_worldP,
												NULL,
												(void*)infoP,
												FilterImage8,
												output_worldP);
						break;
						
					default:
						err = PF_Err_BAD_CALLBACK_PARAM;
						break;
				}
			}		
		}
		ERR2(extraP->cb->checkin_layer_pixels(in_data->effect_ref, PARAM_INPUT));
	}
	return err;
	
}


extern "C" DllExport
PF_Err PluginDataEntryFunction2(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB2 inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT_EXT2(
		inPtr,
		inPluginDataCallBackPtr,
		"NF-Skelton_step1", // Name
		"NF-Skelton_step1", // Match Name
		"NF-Plugins", // Category
		AE_RESERVED_INFO, // Reserved Info
		"EffectMain",	// Entry point
		"https://github.com/bryful");	// support URL

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_dataP,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) 
		{
			case PF_Cmd_ABOUT:
				err = About(in_dataP,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = GlobalSetup(in_dataP,out_data,params,output);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ParamsSetup(in_dataP,out_data,params,output);
				break;
			case PF_Cmd_RENDER:
				err = Render(in_dataP,out_data,params,output);
				break;
			case PF_Cmd_SMART_PRE_RENDER:
				err = PreRender(in_dataP, out_data, (PF_PreRenderExtra*)extra);
				break;
			case PF_Cmd_SMART_RENDER:
				err = SmartRender(in_dataP, out_data, (PF_SmartRenderExtra*)extra);
				break;
		}
	} catch(PF_Err &thrown_err) {
		// Never EVER throw exceptions into AE.
		err = thrown_err;
	}
	return err;
}
