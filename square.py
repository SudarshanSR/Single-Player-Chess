from dataclasses import dataclass


type Rank = int
type File = int


@dataclass(frozen=True)
class Square:
    rank: Rank
    file: File
