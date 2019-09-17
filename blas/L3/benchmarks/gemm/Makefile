#
# Copyright 2019 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# -----------------------------------------------------------------------------
#                            common tool setup

ifeq (,$(XILINX_XRT))
$(info [INFO] XILINX_XRT not defined, using default path /opt/xilinx/xrt)
XILINX_XRT = /opt/xilinx/xrt
endif
export XILINX_XRT

# -----------------------------------------------------------------------------

.PHONY: all

all: clean gemm_bench.exe

# -----------------------------------------------------------------------------
#                                clean up

clean: 
	rm -rf *.exe *.txt

# -----------------------------------------------------------------------------
#                               host setup

SW_API_DIR = ../../include/sw
CXX = g++
CXXFLAGS = -O0 -std=c++11 -fPIC -Wextra -Wall -Wno-ignored-attributes -Wno-unused-parameter -Wno-unused-variable
CXXFLAGS += -I$(XILINX_XRT)/include -I$(SW_API_DIR)
LDFLAGS = -L$(XILINX_XRT)/lib -lz -lstdc++ -lrt -pthread -lxrt_core -ldl -luuid

XFBLAS_dataType ?= short
XFBLAS_numKernels ?= 1

ifneq (${XFBLAS_numKernels}, 1)
  $(info [INFO] Asynchronous Concurrent Execution Is Enabled.)
  CXXFLAGS += -DXFBLAS_LAUNCH_ASYNC
endif

gemm_bench.exe: gemm_bench.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -D XFBLAS_dataType=$(XFBLAS_dataType) -o $@ $^ $(LDFLAGS)

