ADCEN_DIR = ./stereoAdCensus
DISP_DIR = ./disparity
CALIB_DIR = ./calibration
CAPT_DIR = ./capture_stream
EVAL_DIR = ./eval
ADCENGPU_DIR = ./stereoAdCensus/gpu
CANNYMAKE = makeCanny
DILATEMAKE = makeDilate


all: ADCEN CALIB EVAL ADCENGPU DISP CANNY DILATE CAPT

.PHONY:
ADCEN: 
	@pwd 
	@cd $(ADCEN_DIR) && $(MAKE)

.PHONY:
DISP: 
	@cd $(DISP_DIR) && $(MAKE)

.PHONY:
CALIB: 
	@cd $(CALIB_DIR) && $(MAKE)
.PHONY:
CAPT: 
	@cd $(CAPT_DIR) && $(MAKE)
.PHONY:
EVAL: 
	@cd $(EVAL_DIR) && $(MAKE)
.PHONY:
ADCENGPU: 
	@cd $(ADCENGPU_DIR) && $(MAKE)

.PHONY:
CANNY: 
	@cd $(DISP_DIR) && $(MAKE) -f $(CANNYMAKE)

.PHONY:
DILATE:
	@cd $(DISP_DIR) && $(MAKE) -f $(DILATEMAKE)

