#include "ColorSelect.h"
//-----------------------------------------------------------------------------------
extern "C" {
	DllExport
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
			NF_NAME, // Name
			NF_MATCHNAME, // Match Name
			NF_CATEGORY, // Category
			AE_RESERVED_INFO, // Reserved Info
			"EffectMain",	// Entry point
			"https://github.com/bryful");	// support URL

		return result;
	}
	DllExport
		PF_Err
		EffectMain(
			PF_Cmd			cmd,
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP,
			void* extraP)

	{
		PF_Err err = PF_Err_NONE;
		PF_Err err2 = PF_Err_NONE;

		try
		{

			ColorSelect ae;
			ae.in_data = in_dataP;

			switch (cmd) {
			case PF_Cmd_ABOUT:
				err = ae.About(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = ae.GlobalSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = ae.GlobalSetdown(in_dataP);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ae.ParamsSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = ae.SequenceSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = ae.SequenceSetdown(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = ae.SequenceResetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_RENDER:
				err = ae.Render(in_dataP, out_dataP, paramsP, outputP, ID_NUM_PARAMS);
				if (!err)
				{
					ParamInfo info;
					ERR(ae.GetParams(&info));
					ERR(ae.Exec(&info));
				}
				break;
			case PF_Cmd_SMART_PRE_RENDER:
				err = ae.PreRender(in_dataP, out_dataP, reinterpret_cast<PF_PreRenderExtra*>(extraP), ID_NUM_PARAMS, sizeof(ParamInfo));
				if (!err)
				{
					ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
					ERR(ae.SetHostPreRenderData());
					ERR(ae.GetParams(infoP));
					ERR(ae.UnSetPreRenderData());
					ERR(ae.UnlockPreRenderData());

				}
				break;
			case PF_Cmd_SMART_RENDER:
				err = ae.SmartRender(in_dataP, out_dataP, reinterpret_cast<PF_SmartRenderExtra*>(extraP), ID_NUM_PARAMS);
				if (!err) {
					ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
					if (infoP) {
						ERR(ae.Exec(infoP));
						ERR2(ae.UnsetSmartRender());
						ae.UnlockPreRenderData();
					}
					else {
						err = PF_Err_OUT_OF_MEMORY;
					}
				}
				break;
			case PF_Cmd_COMPLETELY_GENERAL:
				err = ae.RespondtoAEGP(in_dataP, out_dataP, paramsP, outputP, extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;
			case PF_Cmd_USER_CHANGED_PARAM:
				err = ae.UserChangedParam(in_dataP,
					out_dataP,
					paramsP,
					outputP,
					reinterpret_cast<PF_UserChangedParamExtra*>(extraP),
					ID_NUM_PARAMS);
				break;
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = ae.QueryDynamicFlags(in_dataP,
					out_dataP,
					paramsP,
					outputP,
					reinterpret_cast<PF_UserChangedParamExtra*>(extraP),
					ID_NUM_PARAMS
					);
				break;
			}
		}
		catch (PF_Err& thrown_err) {
			err = thrown_err;
		}
		return err;
	}
}