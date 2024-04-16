#include "ShiftPer.h"

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_SPRINTF(	out_data->return_msg, 
				"%s, v%d.%d\r%s",
				NF_NAME, 
				MAJOR_VERSION, 
				MINOR_VERSION, 
				NF_DESCRIPTION);

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

	out_data->my_version	= NF_VERSION;
	out_data->out_flags		= NF_OUT_FLAGS;
	out_data->out_flags2 = NF_OUT_FLAGS2;

	return err;
}
static PF_Err
ShiftPixel8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;

	SFTPInfo* infoP = reinterpret_cast<SFTPInfo*>(refcon);

	A_long nx = (xL - infoP->shiftXPixel) % infoP->widthIn;
	if (nx < 0) nx += infoP->widthIn;
	A_long ny = (yL - infoP->shiftYPixel) % infoP->heightIn;
	if (ny < 0) ny += infoP->heightIn;
	PF_Pixel* data = (PF_Pixel*)infoP->data;
	*outP = data[infoP->widthTrueOut * ny + nx];


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
	
	PF_ADD_FLOAT_SLIDERX("ShiftX(%)", 
						-30000,
						30000,
						-200,
						200,
						0,
						1,
						PF_ValueDisplayFlag_PERCENT,
						0,
						SFTP_X);
	PF_ADD_FLOAT_SLIDERX("ShiftY(%)",
		-30000,
		30000,
		-200,
		200,
		0,
		1,
		PF_ValueDisplayFlag_PERCENT,
		0,
		SFTP_Y);
	out_data->num_params = SFTP_NUM_PARAMS;

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

	SFTPInfo			info;
	A_long				linesL	= 0;
	
	AEFX_CLR_STRUCT(info);
	
	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
	info.shiftX = params[SFTP_X]->u.fs_d.value/100;
	info.shiftY = params[SFTP_Y]->u.fs_d.value/100;


	PF_EffectWorld *input = &params[SFTP_INPUT]->u.ld;
	//サイズを獲得
	info.widthIn = input->width;
	info.heightIn = input->height;
	info.widthTrueIn = input->rowbytes/sizeof(PF_Pixel);
	info.widthOut = output->width;
	info.heightOut = output->height;
	info.widthTrueOut = output->rowbytes / sizeof(PF_Pixel);

	info.shiftXPixel = (A_long)((PF_FpLong)info.widthIn * info.shiftX + 0.5);
	info.shiftYPixel = (A_long)((PF_FpLong)info.heightIn * info.shiftY + 0.5);
	info.data = input->data;

	if ((info.shiftXPixel != 0)|| (info.shiftYPixel != 0)) {

		AEFX_SuiteScoper<PF_Iterate8Suite2> iterate8Suite =
			AEFX_SuiteScoper<PF_Iterate8Suite2>(in_dataP,
				kPFIterate8Suite,
				kPFIterate8SuiteVersion2,
				out_data);

		iterate8Suite->iterate(
			in_dataP,
			0,								// progress base
			linesL,							// progress final
			input,	// area - null for all pixels
			NULL,
			(void*)&info,					// refcon - your custom data pointer
			ShiftPixel8,					// pixel function pointer
			output);						// dest
	}
	else {

		AEFX_SuiteScoper<PF_WorldTransformSuite1> worldTransformSuite =
			AEFX_SuiteScoper<PF_WorldTransformSuite1>(in_dataP,
				kPFWorldTransformSuite,
				kPFWorldTransformSuiteVersion1,
				out_data);

		worldTransformSuite->copy(in_dataP->effect_ref,			// This effect ref (unique id)
			&params[SFTP_INPUT]->u.ld,		// Source
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
	PF_ParamDef shiftper_param;
	PF_RenderRequest req = extraP->input->output_request;
	PF_CheckoutResult in_result;
	
	AEFX_CLR_STRUCT(shiftper_param);

	AEFX_SuiteScoper<PF_HandleSuite1> handleSuite = AEFX_SuiteScoper<PF_HandleSuite1>(	in_dataP,
																						kPFHandleSuite,
																						kPFHandleSuiteVersion1,
																						out_dataP);

	PF_Handle infoH	= handleSuite->host_new_handle(sizeof(SFTPInfo));
	
	if (infoH){

		SFTPInfo*infoP = reinterpret_cast<SFTPInfo*>(handleSuite->host_lock_handle(infoH));
		
		if (infoP){

			extraP->output->pre_render_data = infoH;
			
			ERR(PF_CHECKOUT_PARAM(	in_dataP, 
									SFTP_X, 
									in_dataP->current_time, 
									in_dataP->time_step, 
									in_dataP->time_scale, 
									&shiftper_param));
			
			if (!err){
				infoP->shiftX = shiftper_param.u.fs_d.value;
			}
			ERR(PF_CHECKOUT_PARAM(in_dataP,
				SFTP_Y,
				in_dataP->current_time,
				in_dataP->time_step,
				in_dataP->time_scale,
				&shiftper_param));

			if (!err) {
				infoP->shiftY = shiftper_param.u.fs_d.value;
			}
			ERR(extraP->cb->checkout_layer(	in_dataP->effect_ref,
											SFTP_INPUT,
											SFTP_INPUT,
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
	
	SFTPInfo	*infoP = reinterpret_cast<SFTPInfo*>(handleSuite->host_lock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data)));
	
	if (infoP){
		ERR((extraP->cb->checkout_layer_pixels(	in_data->effect_ref, SFTP_INPUT, &input_worldP)));
		ERR(extraP->cb->checkout_output(in_data->effect_ref, &output_worldP));
		
		PF_PixelFormat		format	=	PF_PixelFormat_INVALID;
		
		AEFX_SuiteScoper<PF_WorldSuite2> wsP = AEFX_SuiteScoper<PF_WorldSuite2>(in_data,
																				kPFWorldSuite,
																				kPFWorldSuiteVersion2,
																				out_data);
		// ただコピーするだけ
		err = PF_COPY(input_worldP, output_worldP, NULL, NULL);

		ERR2(extraP->cb->checkin_layer_pixels(in_data->effect_ref, SFTP_INPUT));
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
		"NF-ShiftPer", // Name
		"NF-ShiftPer", // Match Name
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
