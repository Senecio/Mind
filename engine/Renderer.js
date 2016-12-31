/// <reference path="Math.ts" />
var Mind;
(function (Mind) {
    // 颜色
    var Color = (function () {
        function Color(r, g, b, a) {
            this.a = a || 1.0;
            this.r = r;
            this.g = g;
            this.b = b;
        }
        return Color;
    }());
    Mind.Color = Color;
    var IVertexDeclaration;
    (function (IVertexDeclaration) {
        var DataType;
        (function (DataType) {
            DataType[DataType["None"] = 0] = "None";
            DataType[DataType["Byte"] = 1] = "Byte";
            DataType[DataType["Byte2"] = 2] = "Byte2";
            DataType[DataType["Byte3"] = 3] = "Byte3";
            DataType[DataType["Byte4"] = 4] = "Byte4";
            DataType[DataType["UByte"] = 5] = "UByte";
            DataType[DataType["UByte2"] = 6] = "UByte2";
            DataType[DataType["UByte3"] = 7] = "UByte3";
            DataType[DataType["UByte4"] = 8] = "UByte4";
            DataType[DataType["Short"] = 9] = "Short";
            DataType[DataType["Short2"] = 10] = "Short2";
            DataType[DataType["Short3"] = 11] = "Short3";
            DataType[DataType["Short4"] = 12] = "Short4";
            DataType[DataType["UShort"] = 13] = "UShort";
            DataType[DataType["UShort2"] = 14] = "UShort2";
            DataType[DataType["UShort3"] = 15] = "UShort3";
            DataType[DataType["UShort4"] = 16] = "UShort4";
            DataType[DataType["Float"] = 17] = "Float";
            DataType[DataType["Float2"] = 18] = "Float2";
            DataType[DataType["Float3"] = 19] = "Float3";
            DataType[DataType["Float4"] = 20] = "Float4";
        })(DataType = IVertexDeclaration.DataType || (IVertexDeclaration.DataType = {}));
        var DataTypeStride;
        (function (DataTypeStride) {
            DataTypeStride[DataTypeStride["None"] = 0] = "None";
            DataTypeStride[DataTypeStride["Byte"] = 1] = "Byte";
            DataTypeStride[DataTypeStride["Byte2"] = 2] = "Byte2";
            DataTypeStride[DataTypeStride["Byte3"] = 3] = "Byte3";
            DataTypeStride[DataTypeStride["Byte4"] = 4] = "Byte4";
            DataTypeStride[DataTypeStride["UByte"] = 1] = "UByte";
            DataTypeStride[DataTypeStride["UByte2"] = 2] = "UByte2";
            DataTypeStride[DataTypeStride["UByte3"] = 3] = "UByte3";
            DataTypeStride[DataTypeStride["UByte4"] = 4] = "UByte4";
            DataTypeStride[DataTypeStride["Short"] = 2] = "Short";
            DataTypeStride[DataTypeStride["Short2"] = 4] = "Short2";
            DataTypeStride[DataTypeStride["Short3"] = 6] = "Short3";
            DataTypeStride[DataTypeStride["Short4"] = 8] = "Short4";
            DataTypeStride[DataTypeStride["UShort"] = 2] = "UShort";
            DataTypeStride[DataTypeStride["UShort2"] = 4] = "UShort2";
            DataTypeStride[DataTypeStride["UShort3"] = 6] = "UShort3";
            DataTypeStride[DataTypeStride["UShort4"] = 8] = "UShort4";
            DataTypeStride[DataTypeStride["Float"] = 4] = "Float";
            DataTypeStride[DataTypeStride["Float2"] = 8] = "Float2";
            DataTypeStride[DataTypeStride["Float3"] = 12] = "Float3";
            DataTypeStride[DataTypeStride["Float4"] = 16] = "Float4";
        })(DataTypeStride = IVertexDeclaration.DataTypeStride || (IVertexDeclaration.DataTypeStride = {}));
        var Element = (function () {
            ////////////////////////////////////////////////////////////////////////////////
            function Element(attribute, attributeName, dataType) {
                this.attribute = attribute;
                this.attributeName = attributeName.slice();
                this.dataType = dataType;
            }
            return Element;
        }());
        IVertexDeclaration.Element = Element;
    })(IVertexDeclaration = Mind.IVertexDeclaration || (Mind.IVertexDeclaration = {}));
    // 顶点声明实现
    var VertexDeclaration = (function () {
        ////////////////////////////////////////////////////////////////////////////////
        function VertexDeclaration() {
            this.elements = [];
        }
        VertexDeclaration.prototype.GetNumberElement = function () {
            return this.elements.length;
        };
        VertexDeclaration.prototype.GetElementsStride = function () {
            var stride = 0;
            for (var i = 0; i < this.elements.length; ++i) {
                var e = this.elements[i];
                stride += IVertexDeclaration.DataTypeStride[IVertexDeclaration.DataType[e.dataType]];
            }
            return stride;
        };
        VertexDeclaration.prototype.AppendElement = function (element) {
            this.elements.push(element);
        };
        VertexDeclaration.prototype.GetIndexElement = function (index) {
            if (this.elements.length > index) {
                return this.elements[index];
            }
            return null;
        };
        return VertexDeclaration;
    }());
    Mind.VertexDeclaration = VertexDeclaration;
    // 顶点缓存
    var VertexBuffer = (function () {
        ////////////////////////////////////////////////////////////////////////////////
        function VertexBuffer(declaration, usage) {
            this.declaration = declaration;
            this.usage = usage;
        }
        VertexBuffer.prototype.GetDeclaration = function () {
            return this.declaration;
        };
        VertexBuffer.prototype.GetUsage = function () {
            return this.usage;
        };
        VertexBuffer.prototype.SetData = function (data) {
            this.data = data.slice(0);
        };
        return VertexBuffer;
    }());
    Mind.VertexBuffer = VertexBuffer;
    // 索引缓存
    var IndexBuffer = (function () {
        ////////////////////////////////////////////////////////////////////////////////
        function IndexBuffer(usage) {
            this.usage = usage;
        }
        IndexBuffer.prototype.GetUsage = function () {
            return this.usage;
        };
        IndexBuffer.prototype.SetData = function (data) {
            this.data = data.slice(0);
        };
        return IndexBuffer;
    }());
    Mind.IndexBuffer = IndexBuffer;
})(Mind || (Mind = {})); // end Mind
