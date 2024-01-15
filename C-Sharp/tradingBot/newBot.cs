// Created by M.Hankinson
// NOT FOR RE-DISTRIBUTION

using cAlgo.API;
using cAlgo.API.Indicators;

namespace BasicTradingBot

{
    [Robot(TimeZone = TimeZones.UTC, AccessRights = AccessRights.None)]
    public class BasicTradingBot : Robot
    {
        public Bars bars;
        public ExponentialMovingAverage _ema10;
        public ExponentialMovingAverage _ema50;
        public BollingerBands _bb;
        public RelativeStrengthIndex _rsi;
        public Bars _dailySeries;
        public string symbol;
        public bool stopTrading = false;
        public bool wentUnder;
        public bool wentOver;
        
        protected override void OnStart()
        {
            symbol = Chart.Symbol.Name;
            bars = MarketData.GetBars(Chart.TimeFrame, symbol);
            _dailySeries = MarketData.GetBars(TimeFrame.Hour);
            _ema10 = Indicators.ExponentialMovingAverage(bars.ClosePrices, 9);
            _ema50 = Indicators.ExponentialMovingAverage(bars.ClosePrices, 50);
            _bb = Indicators.BollingerBands(bars.ClosePrices, 20, 2, MovingAverageType.TimeSeries);
            _ema10.TimeFrame = _ema50.TimeFrame = _bb.TimeFrame = TimeFrame.Minute15;
            _rsi = Indicators.RelativeStrengthIndex(_dailySeries.ClosePrices, 14);
        }

        protected override void OnTick()
        {
            
            // ** No new orders if 'OnStop()' has been called **
            if (stopTrading)
            {
                return;
            }
        
            // Get positions, symbol price and equity
            bool noPositions = ((Positions.FindAll("Buy Order").Length == 0)&&(Positions.FindAll("Sell Order").Length == 0));
            double pipVal = Symbol.PipValue;
            var volume = Symbol.NormalizeVolumeInUnits(Symbol.QuantityToVolumeInUnits(1));
            double stopLossPips = Account.Equity * 0.00005 / (Symbol.TickSize * Symbol.LotSize * pipVal);
            double takeProfitPips = Account.Equity * 0.0003 / (Symbol.TickSize * Symbol.LotSize * pipVal);
            double stopLossPrice = System.Math.Round((Symbol.Bid + Symbol.Ask) / 2 / Symbol.PipSize) * Symbol.PipSize + stopLossPips * Symbol.PipSize;
            double takeProfitPrice = System.Math.Round((Symbol.Bid + Symbol.Ask) / 2 / Symbol.PipSize) * Symbol.PipSize + takeProfitPips * Symbol.PipSize;
            
            // Get the execution indicator values 
            var ema10Value = _ema10.Result.Last(0);
            var ema50Value = _ema50.Result.Last(0);
            var bbUpperBand = _bb.Top.Last(0);
            var bbLowerBand = _bb.Bottom.Last(0);

            // Get the indicators for the daily trend
            var dailyEma10Value = Indicators.ExponentialMovingAverage(_dailySeries.ClosePrices, 10).Result.Last(0);
            var dailyEma50Value = Indicators.ExponentialMovingAverage(_dailySeries.ClosePrices, 50).Result.Last(0);
            var dailyRsiValue = Indicators.RelativeStrengthIndex(_dailySeries.ClosePrices, 14).Result.Last(0);
            
            // Check the daily trend of the market
            bool isDailyTrendingUp = ((dailyEma10Value > dailyEma50Value) && (dailyRsiValue > 50));
            bool isDailyTrendingDown = ((dailyEma10Value < dailyEma50Value) && (dailyRsiValue < 50));
            
            if ((isDailyTrendingUp)&&(noPositions)) 
            {
                if ((ema10Value < ema50Value) && (Symbol.Bid < bbLowerBand))
                {   
                    wentUnder = true;
                }           
            }
            
            else if ((isDailyTrendingDown)&&(noPositions)) 
            {
                if ((ema10Value > ema50Value) && (Symbol.Ask > bbUpperBand))
                {
                    wentOver = true;
                }
            }
            
            if(wentUnder)
            {
                if ((ema10Value < ema50Value) && (Symbol.Bid > bbLowerBand))
                {
                    if (bars.ClosePrices.Last(1) > bars.ClosePrices.Last(2))
                    {
                        Buy(stopLossPrice, takeProfitPrice, volume, stopLossPips);
                        wentUnder = false;
                    }
                }
            }
            
            if(wentOver) 
            {
                if((ema10Value > ema50Value) && (Symbol.Bid < bbLowerBand))
                {
                    if(bars.ClosePrices.Last(1) < bars.ClosePrices.Last(2))
                    {
                        Sell(stopLossPrice, takeProfitPrice, volume, stopLossPips);
                        wentOver = false;
                    }
                }
            }
        }

        private void Buy(double stopLossPrice, double takeProfitPrice, double volume, double stopLossPips)
        {
            // Execute a buy
            var result = ExecuteMarketOrder(TradeType.Buy, symbol, volume, "Buy Order", stopLossPrice, takeProfitPrice);
            if(result.IsSuccessful)
            {
                // Add trailing stop loss to the position
                double trailingStopLossInPips = stopLossPips;
                double trailingStopLossPrice = Symbol.Ask - trailingStopLossInPips * Symbol.PipSize;
                ModifyPosition(result.Position, trailingStopLossPrice, result.Position.TakeProfit);
            } 
            else if (!result.IsSuccessful) 
            {
                Print("Error opening a buy order: ", result.Error);
            }
        }

        private void Sell(double stopLossPrice, double takeProfitPrice, double volume, double stopLossPips)
        {
            // Execute a sell
            var result = ExecuteMarketOrder(TradeType.Sell, symbol, volume, "Sell Order", stopLossPrice, takeProfitPrice);
            if(result.IsSuccessful)
            {
                // Add trailing stop loss to the position
                double trailingStopLossInPips = stopLossPips;
                double trailingStopLossPrice = Symbol.Ask - trailingStopLossInPips * Symbol.PipSize;
                ModifyPosition(result.Position, trailingStopLossPrice, result.Position.TakeProfit);
            } 
            else if (!result.IsSuccessful) 
            {
                Print("Error opening a sell order: ", result.Error);
            }
        }

        protected override void OnStop() 
        {
            // Cancel any pending orders
            foreach (var order in PendingOrders) {
                CancelPendingOrder(order);
            }
            stopTrading = true;
        }
    }
}
