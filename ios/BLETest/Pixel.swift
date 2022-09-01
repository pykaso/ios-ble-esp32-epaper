import UIKit

struct Pixel {
    var r: Float
    var g: Float
    var b: Float
    var a: Float
    var row: Int
    var col: Int

    init(r: UInt8, g: UInt8, b: UInt8, a: UInt8, row: Int, col: Int) {
        self.r = Float(r)
        self.g = Float(g)
        self.b = Float(b)
        self.a = Float(a)
        self.row = row
        self.col = col
    }

    var color: UIColor {
        return UIColor(
            red: CGFloat(r / 255.0),
            green: CGFloat(g / 255.0),
            blue: CGFloat(b / 255.0),
            alpha: CGFloat(a / 255.0)
        )
    }

    var description: String {
        return "\(r), \(g), \(b), \(a)"
    }
}
