#!/bin/sh
TOPO="dpillar_6_3 dpillar_10_3 dpillar_14_3 dpillar_18_3 dpillar_22_3 dpillar_26_3 dpillar_28_3 dpillar_32_3 dpillar_34_3 dpillar_38_3 dpillar_40_3 dpillar_42_3 dpillar_44_3 dpillar_46_3 dpillar_48_3 dpillar_50_3 dpillar_52_3 dpillar_54_3 dpillar_56_3 dpillar_58_3 dpillar_60_3 dpillar_62_3"
#"dpillar_4_3 dpillar_8_3 dpillar_12_3 dpillar_16_3 dpillar_20_3 dpillar_24_3 dpillar_30_3 dpillar_36_3 dpillar_42_3 dpillar_48_3 dpillar_64_3 dpillar_4_4 dpillar_8_4 dpillar_12_4 dpillar_16_4 dpillar_20_4 dpillar_24_4"
#"knkstar_4_4_4 knkstar_4_5_4 knkstar_4_6_4 knkstar_4_7_4 knkstar_4_8_4 knkstar_4_9_4 knkstar_5_4_4 knkstar_5_5_4 knkstar_5_6_4 knkstar_5_7_4 knkstar_5_8_4 knkstar_6_4_4 knkstar_6_5_4 knkstar_6_6_4"
#"swcube_4_4 swcube_4_5 swcube_4_6 swcube_4_7 swcube_4_8 swcube_4_9 swcube_5_4 swcube_5_5 swcube_5_6 swcube_5_7 swcube_5_8 swcube_6_4 swcube_6_5 swcube_6_6"

for PARAM_TOPO in ${TOPO}
do
    ./build/bin/inrflow topo=${PARAM_TOPO} &
done




