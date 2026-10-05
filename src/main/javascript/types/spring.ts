export interface SpringPageable {
  pageNumber?: number
  pageSize?: number
  sort?: {
    empty: boolean
    sorted: boolean
    unsorted: boolean
  }
  offset?: number
  paged?: boolean
  unpaged?: boolean
}

export interface SpringPageResponse<T> {
  content: T[]
  pageable: SpringPageable | Record<string, never>
  last: boolean
  totalPages: number
  totalElements: number
  size: number
  number: number
  first?: boolean
  numberOfElements?: number
  empty?: boolean
  sort?: {
    empty: boolean
    sorted: boolean
    unsorted: boolean
  }
}
