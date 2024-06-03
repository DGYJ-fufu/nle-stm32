[#ftl]
[#list configs as dt]
[#assign data = dt]
[#assign usedIPs = dt.usedIPs]
[#assign peripheralParams = dt.peripheralParams]
[#assign peripheralGPIOParams = dt.peripheralGPIOParams]
[#assign peripheralDMAParams = dt.peripheralDMAParams]
[#assign peripheralNVICParams = dt.peripheralNVICParams]
[#assign deviceFamily="${dt.family}xx"]
[#assign stm32l100xb_devices  = ["STM32L100C6",   "STM32L100R8",   "STM32L100RB"]]
[#assign stm32l100xba_devices = ["STM32L100C6xxA","STM32L100R8xxA","STM32L100RBxxA"]]
[#assign stm32l100xc_devices  = ["STM32L100RC"]]
[#assign stm32l151xb_devices  = ["STM32L151C6",   "STM32L151R6",   "STM32L151C8",   "STM32L151R8",   "STM32L151V8",   "STM32L151CB",   "STM32L151RB",   "STM32L151VB"]]
[#assign stm32l151xba_devices = ["STM32L151C6xxA","STM32L151R6xxA","STM32L151C8xxA","STM32L151R8xxA","STM32L151V8xxA","STM32L151CBxxA","STM32L151RBxxA","STM32L151VBxxA"]]
[#assign stm32l151xc_devices  = ["STM32L151CC",   "STM32L151RC" ,  "STM32L151UC",   "STM32L151VC"]]
[#assign stm32l151xca_devices = ["STM32L151QC",   "STM32L151RCxxA","STM32L151VCxxA","STM32L151ZC"]]
[#assign stm32l151xd_devices  = ["STM32L151QD",   "STM32L151RD",   "STM32L151VD" ,  "STM32L151ZD"]]
[#assign stm32l151xdx_devices = ["STM32L151VDxxX"]]
[#assign stm32l151xe_devices  = ["STM32L151QE",   "STM32L151RE",   "STM32L151VE",   "STM32L151ZE"]]
[#assign stm32l152xb_devices  = ["STM32L152C6",   "STM32L152R6",   "STM32L152C8",   "STM32L152R8",    "STM32L152V8",   "STM32L152CB",   "STM32L152RB",   "STM32L152VB"]]
[#assign stm32l152xba_devices = ["STM32L152C6xxA","STM32L152R6xxA","STM32L152C8xxA","STM32L152R8xxA", "STM32L152V8xxA","STM32L152CBxxA","STM32L152RBxxA","STM32L152VBxxA"]]
[#assign stm32l152xc_devices  = ["STM32L152CC",   "STM32L152RC",   "STM32L152UC" ,  "STM32L152VC"]]
[#assign stm32l152xca_devices = ["STM32L152QC",   "STM32L152RCxxA","STM32L152VCxxA","STM32L152ZC"]]
[#assign stm32l152xd_devices  = ["STM32L152QD",   "STM32L152RD",   "STM32L152VD",   "STM32L152ZD"]]
[#assign stm32l152xdx_devices = ["STM32L152VDxxX"]]
[#assign stm32l152xe_devices  = ["STM32L152QE",   "STM32L152RE",   "STM32L152VE",   "STM32L152ZE"]]
[#assign stm32l162xc_devices  = ["STM32L162RC",   "STM32L162VC"]]
[#assign stm32l162xca_devices = ["STM32L162RCxxA","STM32L162VCxxA","STM32L162QC",   "STM32L162ZC"]]
[#assign stm32l162xd_devices  = ["STM32L162QD",   "STM32L162RD",   "STM32L162VD",   "STM32L162ZD"]]
[#assign stm32l162xdx_devices = ["STM32L162VDxxX"]]
[#assign stm32l162xe_devices  = ["STM32L162RE",   "STM32L162VE",   "STM32L162ZE"]]
[#assign tmpName= "STM32L1"+"${dt.device[7]}"+"${dt.device[8]}"+"${dt.device[9]}"+"${dt.device[10]}"]
[#if     dt.device?length gt 13]
  [#if     dt.device[13] == 'A']
    [#assign tmpName += "xxA"]
  [#elseif  dt.device[13] == 'X']
    [#assign tmpName += "xxX"]
  [/#if]
[/#if]
[#if     stm32l100xb_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l100xb.s"]
[#elseif stm32l100xba_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l100xba.s"]
[#elseif stm32l100xc_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l100xc.s"]
[#elseif stm32l151xb_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l151xb.s"]
[#elseif stm32l151xba_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l151xba.s"]
[#elseif stm32l151xc_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l151xc.s"]
[#elseif stm32l151xca_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l151xca.s"]
[#elseif stm32l151xd_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l151xd.s"]
[#elseif stm32l151xdx_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l151xdx.s"]
[#elseif stm32l151xe_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l151xe.s"]
[#elseif stm32l152xb_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l152xb.s"]
[#elseif stm32l152xba_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l152xba.s"]
[#elseif stm32l152xc_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l152xc.s"]
[#elseif stm32l152xca_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l152xca.s"]
[#elseif stm32l152xd_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l152xd.s"]
[#elseif stm32l152xdx_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l152xdx.s"]
[#elseif stm32l152xe_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l152xe.s"]
[#elseif stm32l162xc_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l162xc.s"]
[#elseif stm32l162xca_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l162xca.s"]
[#elseif stm32l162xd_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l162xd.s"]
[#elseif stm32l162xdx_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l162xdx.s"]
[#elseif stm32l162xe_devices?seq_contains(tmpName)]
  [#assign startupFile = "startup_stm32l162xe.s"]
[/#if]
[#assign ll_used = 0]
[#assign hal_used = 0]
[#list usedIPs as ip]
[#if peripheralParams.get(ip)?? && peripheralParams.get(ip).get("driver") == ("LL")]
  [#assign ll_used = 1]
[/#if]
[#if ip?starts_with("NVIC") == false]
[#if peripheralParams.get(ip)?? && peripheralParams.get(ip).get("driver") == ("HAL")]
  [#assign hal_used = 1]
[/#if]
[/#if]
[/#list]
[#assign timebase_tim = 0]
[#if timeBaseSource?? && timeBaseSource!="SysTick"]
[#assign timebase_tim = 1]
[/#if]
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!-- ******************************************************************************
 * File Name   : FrameworkCubeMX.gpdsc
 * Date        : ${date}
 * Description : Generator PDSC File generated by STM32CubeMX (DO NOT EDIT!)
 ****************************************************************************** -->
<package xmlns:xs="http://www.w3.org/2001/XMLSchema-instance" schemaVersion="1.0" xs:noNamespaceSchemaLocation="PACK.xsd">
  <vendor>Keil</vendor>
  <name>FrameworkCubeMX</name>
  <description>STM32CubeMX generated pack description</description>
  <url>project-path</url>
  <releases>
    <release version="1.0.0">
     - Generated: ${date}
    </release>
  </releases>
  <generators>
    <generator id="STM32CubeMX" Gvendor="STMicroelectronics" Gtool="STM32CubeMX" Gversion="4.10.0">
      <description>STM32CubeMX Environment</description>
      <select Dname="${(dt.device)}" Dvendor="STMicroelectronics:13"/>
      <command>$SMDK\CubeMX\STM32CubeMXLauncher</command>
      <workingDir>$PRTE\Device\${(dt.device)}</workingDir>
      <project_files>
        <file category="include" name="STCubeGenerated/Inc/"/>
        <file category="source" name="STCubeGenerated/Src/main.c" />
        <file category="header" name="STCubeGenerated/Inc/${deviceFamily?lower_case}_it.h"/>
        <file category="source" name="STCubeGenerated/Src/${deviceFamily?lower_case}_it.c"/>
      </project_files>
    </generator>
  </generators>
  <taxonomy>
    <description Cclass="Device" Cgroup="STM32Cube Framework" generator="STM32CubeMX">STM32Cube Framework</description>
  </taxonomy>
  <conditions>
    <condition id="STCubeMX">
      <description>Condition to include CMSIS core, Device Startup and HAL Drivers components</description>
      <require Dvendor="STMicroelectronics:13" Dname="STM32L0*"/>
      <require Cclass="CMSIS"  Cgroup="CORE"/>
      <require Cclass="Device" Cgroup="Startup"/>
      <require Cclass="Device" Cgroup="STM32Cube HAL"/>
    </condition>
  </conditions>
  <components>
    <bundle Cbundle="STM32CubeMX" Cclass="Device" Cversion="1.0.0">
      <component generator="STM32CubeMX" Cvendor="Keil" Cgroup="STM32Cube Framework" Csub="STM32CubeMX" Cversion="1.1.0" condition="STCubeMX">
        <description>Configuration via STM32CubeMX</description>
        <RTE_Components_h>
[#if ll_used == 1]
          #define RTE_DEVICE_LL_DRIVERS_USED
[/#if]
          #define RTE_DEVICE_FRAMEWORK_CUBE_MX
        </RTE_Components_h>
        <files>
          <file category="header" name="MX_Device.h"/>
          <file category="header" name="STCubeGenerated/Inc/${deviceFamily?lower_case}_hal_conf.h"/>
[#if hal_used == 1]
          <file category="source" name="STCubeGenerated/Src/${deviceFamily?lower_case}_hal_msp.c"/>
[/#if]
[#if timebase_tim == 1]
          <file category="source" name="STCubeGenerated/Src/${deviceFamily?lower_case}_hal_timebase_tim.c"/>
[/#if]
        </files>
      </component>
      <component  Cgroup="Startup">
        <description>System Startup for STMicroelectronics</description>
        <files>
          <file category="source" name="STCubeGenerated/MDK-ARM/${startupFile}" />
          <file category="source" name="STCubeGenerated/Src/system_${deviceFamily?lower_case}.c" />
        </files>
      </component>
    </bundle>
  </components>
</package>
[/#list]
