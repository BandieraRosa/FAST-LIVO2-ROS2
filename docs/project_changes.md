# Project-specific changes

Base: v4rl-ucy/FAST-LIVO2-ROS2 commit `5f98c656831089db0d0e85f604e6a7e20baf94e7`.

## Supplied Livox point timestamps

`preprocess.preserve_point_timestamps` defaults to false. Enabling it preserves each supplied `offset_time` (ns converted to ms in curvature) in the non-feature Avia path, bypassing the upstream 240 kHz synthesized timestamps. Point coordinates, filtering and estimator thresholds remain unchanged.

The project's public Avia triple-return excerpt contains small time corrections. Synthesizing the remaining points as single-return samples extends some scans and causes state-time reversals; the integration replay explicitly enables preservation for this dataset. This does not establish a MID-360 hardware result.

## Build and output layout

Vikit linkage uses the dependencies exported by ament instead of assuming a sibling install path. The integration build applies its verified x86 Eigen/PCL allocation ABI profile externally; ARM builds require their own validation.

Set `LIVO_LOG_DIR` to a run-owned directory to direct trajectory, debug, Colmap and PCD output there. If absent, upstream `ROOT_DIR/Log` remains the default. Integration replay creates the needed directories and records output outside source control. Estimator update and output timestamp behavior are unchanged.

## Regression test

With `BUILD_TESTING=ON`, CTest `preprocess_timestamps` exercises actual preprocessing using a synthetic scan with a timestamp correction. It checks that timestamp preservation retains the source values, the default remains unchanged, geometry/selection agree, and a configured run directory is respected.
