// @ts-check

import { defineConfig } from 'rollup'
import { buildConfigs } from '../../scripts/getRollupConfig.mjs'

export default defineConfig(
  buildConfigs(
    {
      name: 'routing-kit-core',
      jsName: 'RoutingKitCore',
      outputFile: 'index',
      entryFile: 'src/index.ts',
      external: [],
      globals: {},
    },
    ['cjs']
  )
)
