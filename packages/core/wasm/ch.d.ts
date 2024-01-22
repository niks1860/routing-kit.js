export type Vector<T extends number> = {
  new (): Vector<T>
  get(i: T): T
  set(i: T, v: T): void
  size(): T
  resize(n: T, v: T): void
  push_back(v: T): void
}

export type Float = number
export type Uint = number

export type VectorUint = Vector<Uint>
export type VectorFloat = Vector<Float>

type ReadCallback = (view: Int8Array) => void
type WriteCallback = (buffer: Int8Array) => void

export interface CustomizableHeirarchy {
  node_count(): Uint
  get_order(): VectorUint
  get_rank(): VectorUint
  write(callback: WriteCallback): void
}

export interface CustomizableHeirarchyConstructor {
  build(
    tail: VectorUint,
    head: VectorUint,
    weight: VectorUint,
    log?: (msg: string) => void,
    maxPopCount?: number
  ): CustomizableHeirarchy
  build_given_order(
    order: VectorUint,
    tail: VectorUint,
    head: VectorUint,
    weight: VectorUint,
    log?: (msg: string) => void,
    maxPopCount?: number
  ): CustomizableHeirarchy
  build_given_rank(
    rank: VectorUint,
    tail: VectorUint,
    head: VectorUint,
    weight: VectorUint,
    log?: (msg: string) => void,
    maxPopCount?: number
  ): CustomizableHeirarchy
  read(callback: ReadCallback): CustomizableHeirarchy
}

export interface CustomizableHeirarchyQuery {
  new (ch: CustomizableHeirarchy): CustomizableHeirarchyQuery
  reset(): CustomizableHeirarchyQuery
  add_source(node: Uint, distance: Uint): CustomizableHeirarchyQuery
  add_target(node: Uint, distance: Uint): CustomizableHeirarchyQuery
  run(): CustomizableHeirarchyQuery
  get_used_source(): Uint
  get_used_target(): Uint
  get_distance(): Uint
  get_node_path(): VectorUint
  get_arc_path(): VectorUint
  reset_source(): CustomizableHeirarchyQuery
  pin_targets(targets: VectorUint): CustomizableHeirarchyQuery
  run_to_pinned_targets(): CustomizableHeirarchyQuery
  get_distances_to_targets(): VectorUint
  reset_target(): CustomizableHeirarchyQuery
  pin_sources(sources: VectorUint): CustomizableHeirarchyQuery
  run_to_pinned_sources(): CustomizableHeirarchyQuery
  get_distances_to_sources(): VectorUint
  get_used_sources_to_targets(): VectorUint
  get_used_targets_to_sources(): VectorUint
}

export interface CustomizableHeirarchyQueryConstructor {
  new (ch: CustomizableHeirarchy): CustomizableHeirarchyQuery
}

export interface CustomizableHeirarchyModuleInstance {
  VectorUint: VectorUint
  VectorFloat: VectorFloat
  CH: CustomizableHeirarchyConstructor
  CHQuery: CustomizableHeirarchyQueryConstructor
}

export interface CustomizableHeirarchyModuleArgs {}

function CustomizableHeirarchyModuleCtor(
  args: CustomizableHeirarchyModuleArgs
): Promise<CustomizableHeirarchyModuleInstance>

export default CustomizableHeirarchyModuleCtor
