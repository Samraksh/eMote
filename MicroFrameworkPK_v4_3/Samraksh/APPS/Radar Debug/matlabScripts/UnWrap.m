function UnPhase = UnWrap(Phase, Min,Max)

Range = Max - Min;

Delta = diff(Phase);
UnDelta = [0; mod(Delta - Min,Range) + Min];
UnPhase = cumsum(UnDelta);