//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.3603
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace SimpleWPFApplication
{
    
    internal partial class Resources
    {
        private static System.Resources.ResourceManager manager;
        internal static System.Resources.ResourceManager ResourceManager
        {
            get
            {
                if ((Resources.manager == null))
                {
                    Resources.manager = new System.Resources.ResourceManager("SimpleWPFApplication.Resources", typeof(Resources).Assembly);
                }
                return Resources.manager;
            }
        }
        internal static Microsoft.SPOT.Bitmap GetBitmap(Resources.BitmapResources id)
        {
            return ((Microsoft.SPOT.Bitmap)(Microsoft.SPOT.ResourceUtility.GetObject(ResourceManager, id)));
        }
        internal static Microsoft.SPOT.Font GetFont(Resources.FontResources id)
        {
            return ((Microsoft.SPOT.Font)(Microsoft.SPOT.ResourceUtility.GetObject(ResourceManager, id)));
        }
        internal static string GetString(Resources.StringResources id)
        {
            return ((string)(Microsoft.SPOT.ResourceUtility.GetObject(ResourceManager, id)));
        }
        [System.SerializableAttribute()]
        internal enum BitmapResources : short
        {
            Scrollable_Panel_Icon = -20164,
            Canvas_Panel_Icon = -9105,
            Vertical_Stack_Panel_Icon = -7672,
            Horizontal_Stack_Panel_Icon_Small = -6042,
            Scrollable_Panel_Icon_Small = 4749,
            Free_Drawing_Panel_Icon = 4854,
            Free_Drawing_Panel_Icon_Small = 15133,
            Horizontal_Stack_Panel_Icon = 15925,
            Canvas_Panel_Icon_Small = 20403,
            Vertical_Stack_Panel_Icon_Small = 31390,
        }
        [System.SerializableAttribute()]
        internal enum StringResources : short
        {
            DrawTextSample = -12253,
            MenuScrolling = -8879,
            MenuSelection = 9947,
            ScrollableTextTitle = 14311,
            ReturnToMenu = 29183,
            ScrollableText = 31192,
        }
        [System.SerializableAttribute()]
        internal enum FontResources : short
        {
            NinaBFont = 1845,
            SmallFont = 15889,
        }
    }
}
